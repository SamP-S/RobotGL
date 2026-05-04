"""Tests for trajectory generation (headless)."""

import numpy as np
import pytest

from robotgl.engine.trajectory import (
    JointTrajectory,
    CartesianTrajectory,
    CircularTrajectory,
    SplineTrajectory,
)


def test_joint_trajectory_start_end() -> None:
    """Test joint trajectory starts and ends correctly."""
    start = [0.0, 0.0]
    end = [1.0, 2.0]
    traj = JointTrajectory(start, end, duration=1.0, dt=0.001)
    assert np.allclose(traj.sample(0.0), start)
    assert np.allclose(traj.sample(1.0), end)


def test_joint_trajectory_midpoint() -> None:
    """Test joint trajectory midpoint."""
    start = [0.0, 0.0]
    end = [1.0, 2.0]
    traj = JointTrajectory(start, end, duration=1.0)
    mid = traj.sample(0.5)
    assert np.allclose(mid, [0.5, 1.0])


def test_joint_trajectory_invalid() -> None:
    """Test error on mismatched lengths."""
    with pytest.raises(ValueError):
        JointTrajectory([0.0], [1.0, 2.0], duration=1.0)


def test_cartesian_trajectory_position() -> None:
    """Test cartesian trajectory interpolates position."""
    start = np.eye(4)
    start[0, 3] = 0.0
    end = np.eye(4)
    end[0, 3] = 1.0
    traj = CartesianTrajectory(start, end, duration=1.0)
    mid = traj.sample(0.5)
    assert np.allclose(mid[0, 3], 0.5)


def test_circular_trajectory_xy() -> None:
    """Test circular trajectory in XY plane."""
    center = np.array([0.0, 0.0, 0.0])
    traj = CircularTrajectory(
        center, radius=1.0, start_angle=0.0, end_angle=np.pi / 2, duration=1.0
    )
    start_point = traj.sample(0.0)
    end_point = traj.sample(1.0)
    assert np.allclose(start_point[:2, 3], [1.0, 0.0], atol=0.01)
    assert np.allclose(end_point[:2, 3], [0.0, 1.0], atol=0.01)


def test_spline_trajectory_waypoints() -> None:
    """Test spline passes through waypoints."""
    wp1 = np.array([0.0, 0.0])
    wp2 = np.array([1.0, 1.0])
    wp3 = np.array([2.0, 0.0])
    traj = SplineTrajectory([wp1, wp2, wp3], [1.0, 1.0])
    assert np.allclose(traj.sample(0.0), wp1)
    assert np.allclose(traj.sample(1.0), wp2, atol=0.1)
    assert np.allclose(traj.sample(2.0), wp3)


def test_trajectory_step() -> None:
    """Test trajectory step function."""
    traj = JointTrajectory([0.0], [1.0], duration=0.2, dt=0.1)
    sample1, done1 = traj.step()
    assert not done1
    assert np.allclose(sample1, [0.5], atol=0.01)
    sample2, done2 = traj.step()
    assert done2
    assert np.allclose(sample2, [1.0], atol=0.01)


def test_trajectory_reset() -> None:
    """Test trajectory reset."""
    traj = JointTrajectory([0.0], [1.0], duration=1.0)
    traj.step()
    traj.reset()
    assert traj._current_time == 0.0
