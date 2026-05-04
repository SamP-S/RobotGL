"""TCP/IP robot interface for robotgl."""

import socket
from typing import Optional

from robotgl.robot_interfaces.base import BaseRobotInterface


class TcpInterface(BaseRobotInterface):
    """TCP socket communication interface for robots."""

    def __init__(
        self,
        host: str,
        port: int,
        timeout: float = 5.0,
    ) -> None:
        self._host = host
        self._port = port
        self._timeout = timeout
        self._socket: Optional[socket.socket] = None

    def connect(self) -> bool:
        """Connect to robot via TCP."""
        try:
            self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._socket.settimeout(self._timeout)
            self._socket.connect((self._host, self._port))
            return True
        except socket.error:
            return False

    def disconnect(self) -> None:
        """Close TCP connection."""
        if self._socket:
            try:
                self._socket.close()
            except socket.error:
                pass
            self._socket = None

    def set_joint_angles(self, angles: list[float]) -> bool:
        """Send joint angles as comma-separated values."""
        if not self._socket:
            return False
        try:
            data = ",".join(str(a) for a in angles) + "\n"
            self._socket.sendall(data.encode())
            return True
        except socket.error:
            return False

    def get_joint_angles(self) -> Optional[list[float]]:
        """Read joint angles from TCP socket."""
        if not self._socket:
            return None
        try:
            self._socket.settimeout(1.0)
            data = self._socket.recv(1024).decode().strip()
            if data:
                return [float(x) for x in data.split(",")]
            return None
        except (socket.error, ValueError):
            return None

    def home(self) -> bool:
        """Send home command."""
        if not self._socket:
            return False
        try:
            self._socket.sendall(b"HOME\n")
            return True
        except socket.error:
            return False
