import serial
import time
import re
from collections import deque
import numpy as np


class ArmController:
    """Non-blocking, queued Arm controller.

    Behavior:
    - Commands are enqueued (set_pose / goto / stop) and sent only when the
      controller is idle (not waiting for ACCEPTED/COMPLETED replies).
    - `poll()` must be called frequently by the main loop to consume serial
      responses and update internal state.
    - `update()` should be called frequently by the main loop to send the
      next queued command when idle.

    Public methods:
    - enqueue_set_pose(idx, angles)
    - enqueue_goto(idx)
    - enqueue_stop()
    - poll()  # read serial and process responses
    - update()  # attempt to send next queued command if ready

    This class intentionally avoids blocking on the serial port so the main
    loop (visualiser, etc.) can continue running.
    """

    MAX_POSES = 32

    RE_ACCEPTED = re.compile(r"<ACCEPTED\s+id=(\d+)>")
    RE_COMPLETED = re.compile(r"<COMPLETED\s+id=(\d+)>")
    RE_STOPPED = re.compile(r"<STOPPED\s+id=(\d+)>")
    RE_REJECTED = re.compile(r"REJECTED")

    def __init__(self, port):
        # Serial port may throw if not available; let caller handle exceptions
        self.ser = serial.Serial(port, baudrate=115200, timeout=0.2)
        self.port = port
        # give Arduino bootloader time if needed
        time.sleep(2)
        self.poses = [[0.0] * 5 for _ in range(32)]

        # runtime state
        self.readyForMotion = True  # logically ready to accept new high-level actions
        self.currentPose = 0

        # queue of pending commands
        self._queue = deque()

        # waiting flags for responses from firmware
        self._waiting_for_accept = False
        self._waiting_for_complete = False
        self._waiting_id = None

        # last error / message for external inspection
        self.last_messages = deque(maxlen=20)

    # --------------------------- enqueue API ---------------------------
    def enqueue_set_pose(self, idx, angles):
        if idx < 1 or idx >= self.MAX_POSES:
            raise ValueError(f"Pose index {idx} out of range (1-{self.MAX_POSES-1})")
        # Convert angles from radians to degrees
        angles_deg = [a * 180.0 / np.pi for a in angles]
        self.poses[idx] = angles_deg
        self._queue.append({"cmd": "set_pose", "id": idx, "angles": angles_deg})

    def enqueue_goto(self, idx):
        if idx < 0 or idx >= self.MAX_POSES:
            raise ValueError(f"Pose index {idx} out of range (0-{self.MAX_POSES-1})")
        self._queue.append({"cmd": "goto", "id": idx})

    def enqueue_stop(self):
        self._queue.append({"cmd": "stop"})

    # --------------------------- I/O primitives --------------------------
    def _send_raw(self, cmd):
        """Send raw command immediately to serial (internal).

        Caller must ensure serial write is safe (i.e., we intend to send a
        command now). This does not block waiting for responses.
        """
        try:
            self.ser.write((cmd + "\n").encode())
            print(f"[{self.port}] OUT: {cmd}")
        except Exception as e:
            # record and propagate minimal info -- don't raise in poll/update
            self.last_messages.append(f"WRITE_ERROR: {e}")

    # keep for compatibility but avoid using it in main loop
    def wait_for(self, expected, timeout=None):
        """Blocking wait kept for tests/legacy only. Not used by main loop.

        Prefer non-blocking poll() + update() in your main loop.
        """
        start = time.time()
        while True:
            line = self.ser.readline().decode().strip()
            if not line:
                if timeout and (time.time() - start) > timeout:
                    raise TimeoutError("wait_for timed out")
                continue
            if expected in line:
                return line
            if "REJECTED" in line:
                raise RuntimeError(line)

    # --------------------------- polling -------------------------------
    def poll(self):
        """Non-blocking: read all available lines from serial and process."""
        try:
            while self.ser.in_waiting > 0:
                raw = self.ser.readline()
                try:
                    line = raw.decode().strip()
                except Exception:
                    line = str(raw)
                if not line:
                    continue
                self._handle_line(line)
        except Exception as e:
            # record serial errors and continue; don't raise to avoid breaking main loop
            self.last_messages.append(f"POLL_ERROR: {e}")

    def _handle_line(self, line):
        """Parse a received line and update state machine flags."""
        self.last_messages.append(line)
        print(f"[{self.port}] IN: {line}")

        # REJECTED
        if self.RE_REJECTED.search(line):
            # firmware refused the last command; clear waiting flags so next command
            # can be attempted. Caller may inspect last_messages for details.
            self._waiting_for_accept = False
            self._waiting_for_complete = False
            self._waiting_id = None
            self.readyForMotion = True
            return

        m = self.RE_ACCEPTED.search(line)
        if m:
            rid = int(m.group(1))
            # ACCEPTED means the controller has queued the motion
            if self._waiting_for_accept and self._waiting_id == rid:
                self._waiting_for_accept = False
            return

        m = self.RE_COMPLETED.search(line)
        if m:
            rid = int(m.group(1))
            if self._waiting_for_complete and self._waiting_id == rid:
                self._waiting_for_complete = False
                self._waiting_id = None
                self.currentPose = rid
                self.readyForMotion = True
                print(f"pose id={rid} angles={self.poses[rid]}")
            return

        m = self.RE_STOPPED.search(line)
        if m:
            rid = int(m.group(1))
            # STOPPED indicates movement halted; update current pose and clear waits
            self._waiting_for_accept = False
            self._waiting_for_complete = False
            self._waiting_id = None
            self.currentPose = rid
            self.readyForMotion = True
            return

        # other messages are stored for user inspection

    # --------------------------- update loop ---------------------------
    def is_waiting(self):
        return self._waiting_for_accept or self._waiting_for_complete

    def update(self):
        """Attempt to send next queued command when controller is idle.

        This is non-blocking: it will only send a command if the controller is
        not currently waiting for a response.
        """
        # do not send new commands while waiting for responses
        if self.is_waiting():
            return

        if not self.readyForMotion:
            return

        if not self._queue:
            return

        item = self._queue.popleft()
        cmd = item.get("cmd")
        if cmd == "set_pose":
            idx = item["id"]
            angles = item["angles"]
            msg = f"<SET_POSE id={idx} angles=" + ";".join(f"{a:.3f}" for a in angles) + ">"
            self._send_raw(msg)
            # assume we get an ACCEPTED; but we don't block waiting.
            self._waiting_for_accept = True
            self._waiting_for_complete = False
            self._waiting_id = idx
            # set_pose doesn't start motion necessarily, allow queuing
            self.readyForMotion = True

        elif cmd == "goto":
            idx = item["id"]
            msg = f"<GO_TO id={idx}>"
            self._send_raw(msg)
            # for goto we expect both ACCEPTED then COMPLETED
            self._waiting_for_accept = True
            self._waiting_for_complete = True
            self._waiting_id = idx
            # once motion begins the arm is not ready for more motions
            self.readyForMotion = False

        elif cmd == "stop":
            msg = "<STOP>"
            self._send_raw(msg)
            # expect STOPPED reply referring to current pose
            self._waiting_for_accept = False
            self._waiting_for_complete = True
            self._waiting_id = self.currentPose
            self.readyForMotion = False

        else:
            # unknown command: ignore
            self.last_messages.append(f"UNKNOWN_CMD: {item}")

    def tick(self):
        """Convenience: poll + update in one call."""
        self.poll()
        self.update()

    # convenience: drain and return queued length
    def getQueueSize(self):
        return len(self._queue)

        