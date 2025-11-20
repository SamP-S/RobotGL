from collections import deque
from typing import Optional, Iterable

from arm_controller import ArmController


class DualArmController:
    """Middleware that encapsulates two ArmController instances.

    Responsibilities:
    - Duplicate high-level commands to both controllers.
    - Expose a combined readyForMotion which is True only when both
      underlying controllers report ready.
    - Provide tick()/poll()/update() convenience that forwards to both
      controllers and keeps simple aggregated state.

    Usage:
        base = ArmController("/dev/ttyUSB0")
        fore = ArmController("/dev/ttyUSB1")
        dual = DualArmController(base, fore)
        dual.enqueue_goto(1)
        while not dual.readyForMotion:
            dual.tick()

    The wrapper accepts either already-created ArmController objects or
    strings (port names) in which case it will instantiate ArmController.
    """

    def __init__(self, port_base: str, port_forearm: str):
        """Create controller pair from two serial port strings.

        Args:
            port_base: serial port for the base controller (e.g. "COM4" or "/dev/ttyUSB0")
            port_forearm: serial port for the forearm controller (e.g. "COM11")
        The underlying ArmController instances are stored as `ctrl_base` and
        `ctrl_forearm` to make their roles explicit.
        """
        # instantiate the two ArmController objects and name them clearly
        self.ctrl_base = ArmController(port_base)
        self.ctrl_forearm = ArmController(port_forearm)

        # aggregated message history (merged view)
        self.last_messages = deque(maxlen=100)

    # ------------------ command duplication API ------------------
    def enqueue_set_pose(self, idx: int, base_angles: Iterable[float], forearm_angles: Optional[Iterable[float]] = None):
        """Enqueue SET_POSE to both controllers.

        If forearm_angles is None the same angles will be sent to both.
        """
        self.ctrl_base.enqueue_set_pose(idx, base_angles)
        if forearm_angles is None:
            self.ctrl_forearm.enqueue_set_pose(idx, base_angles)
        else:
            self.ctrl_forearm.enqueue_set_pose(idx, forearm_angles)

    def enqueue_goto(self, idx: int):
        """Enqueue GO_TO to both controllers (duplicate command)."""
        self.ctrl_base.enqueue_goto(idx)
        self.ctrl_forearm.enqueue_goto(idx)

    def enqueue_stop(self):
        """Enqueue STOP to both controllers."""
        self.ctrl_base.enqueue_stop()
        self.ctrl_forearm.enqueue_stop()

    # ------------------ forwarding/tick API ------------------
    def poll(self):
        """Forward poll to both controllers and aggregate messages."""
        self.ctrl_base.poll()
        self.ctrl_forearm.poll()
        # merge recent messages into wrapper deque
        for m in list(self.ctrl_base.last_messages):
            self.last_messages.append(f"base: {m}")
        for m in list(self.ctrl_forearm.last_messages):
            self.last_messages.append(f"forearm: {m}")

    def update(self):
        """Forward update to both controllers."""
        # We call update on both; order doesn't matter but keep consistent
        self.ctrl_base.update()
        self.ctrl_forearm.update()

    def tick(self):
        """Convenience: poll then update both controllers."""
        # poll first to clear incoming responses, then allow sending
        self.poll()
        self.update()

    # ------------------ aggregated state ------------------
    @property
    def readyForMotion(self) -> bool:
        """True only when both controllers are ready for motion."""
        return bool(self.ctrl_base.readyForMotion and self.ctrl_forearm.readyForMotion)

    def is_waiting(self) -> bool:
        """True if either underlying controller is waiting for responses."""
        return bool(self.ctrl_base.is_waiting() or self.ctrl_forearm.is_waiting())

    @property
    def currentPose(self):
        """Return tuple of (base.currentPose, forearm.currentPose)."""
        return (self.ctrl_base.currentPose, self.ctrl_forearm.currentPose)

    def queued_count(self) -> int:
        return self.ctrl_base.queued_count() + self.ctrl_forearm.queued_count()

    # small helpers for debugging
    def pop_last_messages(self, n: int = 10):
        """Return up to `n` most recent aggregated messages and clear them."""
        out = []
        for _ in range(min(n, len(self.last_messages))):
            out.append(self.last_messages.popleft())
        return out
