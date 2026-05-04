"""Tests for RobotModel (headless, no display needed)."""

import numpy as np
import pytest

from robotgl.engine.dh_loader import DHLoader
from robotgl.engine.robot_model import RobotModel


@pytest.fixture
def robot() -> RobotModel:
    """Load 2DOF test robot."""
    return RobotModel.from_json("tests/test_data/2dof_md.json")


def test_load_json(robot: RobotModel) -> None:
    """Test MDH JSON loading."""
    assert robot.name == "2DOF_Planar"
    assert robot.num_joints == 2


def test_joint_limits(robot: RobotModel) -> None:
    """Test joint limits are parsed."""
    limits = robot.joint_limits
    assert len(limits) == 2
    assert limits[0] == [-3.14, 3.14]


def test_fk_identity(robot: RobotModel) -> None:
    """Test FK at zero pose returns identity (with MDH offsets)."""
    pose = robot.fk([0.0, 0.0])
    assert pose.shape == (4, 4)
    # At zero angles, end-effector should be at (1.0, 0, 0) for two 0.5m links
    assert np.allclose(pose[0, 3], 1.0, atol=0.01)


def test_fk_set_angles(robot: RobotModel) -> None:
    """Test FK with specific joint angles."""
    pose = robot.fk([np.pi / 2, 0.0])
    assert pose.shape == (4, 4)


def test_set_joint_angles(robot: RobotModel) -> None:
    """Test setting joint angles."""
    robot.set_joint_angles([0.5, -0.5])
    assert np.allclose(robot.current_q, [0.5, -0.5])


def test_ik_roundtrip(robot: RobotModel) -> None:
    """Test IK can recover joint angles from FK pose."""
    q_original = [0.5, -0.3]
    pose = robot.fk(q_original)
    q_recovered = robot.ik(pose)
    assert np.allclose(q_recovered, q_original, atol=0.01)


def test_invalid_joint_count(robot: RobotModel) -> None:
    """Test error on wrong number of joints."""
    with pytest.raises(ValueError):
        robot.set_joint_angles([0.0])
