"""Serial (UART) robot interface for robotgl."""

import time
from typing import Optional

from serial import Serial, SerialException

from robotgl.robot_interfaces.base import BaseRobotInterface


class SerialInterface(BaseRobotInterface):
    """Serial communication interface for robots."""

    def __init__(
        self,
        port: str,
        baudrate: int = 9600,
        timeout: float = 1.0,
    ) -> None:
        self._port = port
        self._baudrate = baudrate
        self._timeout = timeout
        self._serial: Optional[Serial] = None

    def connect(self) -> bool:
        """Open serial connection."""
        try:
            self._serial = Serial(
                port=self._port,
                baudrate=self._baudrate,
                timeout=self._timeout,
            )
            time.sleep(2)  # Wait for connection to establish
            return True
        except SerialException:
            return False

    def disconnect(self) -> None:
        """Close serial connection."""
        if self._serial:
            self._serial.close()
            self._serial = None

    def set_joint_angles(self, angles: list[float]) -> bool:
        """Send joint angles as comma-separated values."""
        if not self._serial or not self._serial.is_open:
            return False
        try:
            data = ",".join(str(a) for a in angles) + "\n"
            self._serial.write(data.encode())
            return True
        except SerialException:
            return False

    def get_joint_angles(self) -> Optional[list[float]]:
        """Read joint angles from serial (expects comma-separated values)."""
        if not self._serial or not self._serial.is_open:
            return None
        try:
            if self._serial.in_waiting:
                line = self._serial.readline().decode().strip()
                return [float(x) for x in line.split(",")]
            return None
        except (SerialException, ValueError):
            return None

    def home(self) -> bool:
        """Send home command (assumes robot supports 'HOME' command)."""
        if not self._serial or not self._serial.is_open:
            return False
        try:
            self._serial.write(b"HOME\n")
            return True
        except SerialException:
            return False
