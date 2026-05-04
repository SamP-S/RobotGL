"""Tests for BaseRobotInterface (headless, using mocks)."""

from abc import ABC
from typing import Optional

import pytest

from robotgl.robot_interfaces.base import BaseRobotInterface


class MockInterface(BaseRobotInterface):
    """Mock interface for testing base class."""

    def __init__(self) -> None:
        self.connected = False
        self.angles: Optional[list[float]] = None

    def connect(self) -> bool:
        self.connected = True
        return True

    def disconnect(self) -> None:
        self.connected = False

    def set_joint_angles(self, angles: list[float]) -> bool:
        self.angles = angles.copy()
        return True

    def get_joint_angles(self) -> Optional[list[float]]:
        return self.angles

    def home(self) -> bool:
        self.angles = [0.0] * 6
        return True


def test_mock_interface_connect() -> None:
    """Test mock connect."""
    iface = MockInterface()
    assert iface.connect()
    assert iface.connected


def test_mock_interface_disconnect() -> None:
    """Test mock disconnect."""
    iface = MockInterface()
    iface.connect()
    iface.disconnect()
    assert not iface.connected


def test_mock_interface_set_joint_angles() -> None:
    """Test setting joint angles."""
    iface = MockInterface()
    iface.connect()
    assert iface.set_joint_angles([1.0, 2.0, 3.0])
    assert iface.angles == [1.0, 2.0, 3.0]


def test_mock_interface_get_joint_angles() -> None:
    """Test getting joint angles."""
    iface = MockInterface()
    iface.connect()
    iface.set_joint_angles([0.5, 0.5])
    assert iface.get_joint_angles() == [0.5, 0.5]


def test_mock_interface_home() -> None:
    """Test home command."""
    iface = MockInterface()
    iface.connect()
    iface.set_joint_angles([1.0, 2.0])
    assert iface.home()
    assert iface.angles == [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]


def test_base_interface_is_abstract() -> None:
    """Test BaseRobotInterface cannot be instantiated."""
    with pytest.raises(TypeError):
        BaseRobotInterface()
