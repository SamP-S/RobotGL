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
        dual = DualArmController("/dev/ttyUSB0", "/dev/ttyUSB1")
        dual.enqueue_goto(1)
        while True:
            dual.tick()
            if dual.readyForMotion:
                dual.enqueue_goto(2)

    The wrapper accepts either already-created ArmController objects or
    strings (port names) in which case it will instantiate ArmController.
    """

    def __init__(self, port_base: str, port_forearm: str):
        """Create controller pair from two serial port strings.
        Args:
            port_base: serial port for the base controller (e.g. "COM4" or "/dev/ttyUSB0")
            port_forearm: serial port for the forearm controller (e.g. "COM11")
        """
        self.ctrl_base = ArmController(port_base)
        self.ctrl_forearm = ArmController(port_forearm)

    # ------------------ command duplication API ------------------
    def enqueue_set_pose(self, idx: int, angles: Iterable[float] = None):
        """Enqueue SET_POSE to both controllers."""
        self.ctrl_base.enqueue_set_pose(idx, angles)
        self.ctrl_forearm.enqueue_set_pose(idx, angles)

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


    def update(self):
        """Forward update to both controllers."""
        # synchronize updates: only update if neither is waiting
        if self.is_waiting():
            return
        
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
        """True if either controller is waiting for responses."""
        return bool(self.ctrl_base.is_waiting() or self.ctrl_forearm.is_waiting())


    def getCurrentPoses(self):
        """Return tuple of (base.currentPose, forearm.currentPose)."""
        return (self.ctrl_base.currentPose, self.ctrl_forearm.currentPose)

    def getQueueSizes(self):
        return (self.ctrl_base.getQueueSize(), self.ctrl_forearm.getQueueSize())
