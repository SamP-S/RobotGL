"""Base robot interface for robotgl."""

from abc import ABC, abstractmethod
from typing import Optional


class BaseRobotInterface(ABC):
    """Abstract base class for robot communication interfaces."""

    @abstractmethod
    def connect(self) -> bool:
        """Connect to the robot.

        Returns:
            True if connection successful, False otherwise.
        """
        pass

    @abstractmethod
    def disconnect(self) -> None:
        """Disconnect from the robot."""
        pass

    @abstractmethod
    def set_joint_angles(self, angles: list[float]) -> bool:
        """Send joint angles to robot.

        Args:
            angles: List of joint angles in radians.

        Returns:
            True if successful, False otherwise.
        """
        pass

    @abstractmethod
    def get_joint_angles(self) -> Optional[list[float]]:
        """Read current joint angles from robot.

        Returns:
            List of joint angles in radians, or None if read fails.
        """
        pass

    @abstractmethod
    def home(self) -> bool:
        """Send robot to home/zero position.

        Returns:
            True if successful, False otherwise.
        """
        pass
