"""Tests for main application (headless)."""

import argparse
from unittest.mock import patch, MagicMock

import pytest

from robotgl.main import create_robot_interface, main
from robotgl.robot_interfaces.base import BaseRobotInterface


def test_create_serial_interface() -> None:
    """Test creating serial interface."""
    iface = create_robot_interface("serial", port="/dev/ttyUSB0")
    from robotgl.robot_interfaces.serial_interface import SerialInterface

    assert isinstance(iface, SerialInterface)


def test_create_tcp_interface() -> None:
    """Test creating TCP interface."""
    iface = create_robot_interface("tcp", host="192.168.1.1", port=8080)
    from robotgl.robot_interfaces.tcp_interface import TcpInterface

    assert isinstance(iface, TcpInterface)


def test_create_none_interface() -> None:
    """Test creating none interface."""
    iface = create_robot_interface("none")
    assert iface is None


def test_create_invalid_interface() -> None:
    """Test error on invalid interface type."""
    with pytest.raises(ValueError):
        create_robot_interface("invalid")


@patch("robotgl.main.RobotModel")
@patch("robotgl.main.SimulationLoop")
def test_main_headless(mock_sim: MagicMock, mock_robot: MagicMock) -> None:
    """Test main runs in headless mode."""
    mock_robot.from_json.return_value = MagicMock()
    mock_robot.from_json.return_value.name = "TestRobot"

    with patch("sys.argv", ["robotgl", "test.json", "--headless"]):
        with patch("robotgl.main.run_headless") as mock_run:
            main()
            mock_run.assert_called_once()


@patch("robotgl.main.RobotModel")
def test_main_invalid_robot(mock_robot: MagicMock) -> None:
    """Test main handles invalid robot JSON."""
    mock_robot.from_json.side_effect = FileNotFoundError("File not found")

    with patch("sys.argv", ["robotgl", "invalid.json"]):
        with pytest.raises(SystemExit):
            main()
