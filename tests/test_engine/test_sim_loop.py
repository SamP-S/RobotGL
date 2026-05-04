"""Tests for SimulationLoop (headless)."""

import time

import numpy as np
import pytest

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.engine.trajectory import JointTrajectory


@pytest.fixture
def sim() -> SimulationLoop:
    """Create simulation with 2DOF robot."""
    robot = RobotModel.from_json("tests/test_data/2dof_md.json")
    return SimulationLoop(robot, dt=0.001, speed=1.0)


def test_sim_initial_state(sim: SimulationLoop) -> None:
    """Test simulation starts in correct state."""
    assert not sim.is_running
    assert sim.current_q == [0.0, 0.0]
    assert sim.sim_time == 0.0


def test_sim_start_stop(sim: SimulationLoop) -> None:
    """Test start and stop."""
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1)
    sim.start(traj)
    assert sim.is_running
    sim.stop()
    assert not sim.is_running


def test_sim_pause_resume(sim: SimulationLoop) -> None:
    """Test pause and resume."""
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1)
    sim.start(traj)
    sim.pause()
    assert not sim.is_running
    sim.start()
    assert sim.is_running


def test_sim_step_with_trajectory(sim: SimulationLoop) -> None:
    """Test stepping with a trajectory."""
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1, dt=0.05)
    sim.start(traj)
    done, q = sim.step()
    assert q is not None
    assert not done
    assert len(q) == 2


def test_sim_completes_trajectory(sim: SimulationLoop) -> None:
    """Test simulation completes trajectory."""
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1, dt=0.05)
    sim.start(traj)
    done, _ = sim.step()
    assert not done
    done, _ = sim.step()
    assert done
    assert sim.current_q == [1.0, 1.0]


def test_sim_callback(sim: SimulationLoop) -> None:
    """Test callback is called."""
    callback_data = []

    def callback(q: list[float], t: float) -> None:
        callback_data.append((q.copy(), t))

    sim.add_callback(callback)
    traj = JointTrajectory([0.0, 0.0], [0.5, 0.5], duration=0.1, dt=0.1)
    sim.start(traj)
    sim.step()
    assert len(callback_data) > 0
    assert len(callback_data[0][0]) == 2


def test_sim_speed_control(sim: SimulationLoop) -> None:
    """Test speed multiplier affects sim time."""
    sim.speed = 2.0
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1, dt=0.05)
    sim.start(traj)
    _, _ = sim.step()
    assert sim.sim_time > sim.dt  # Should advance by speed * dt


def test_sim_run_until_done(sim: SimulationLoop) -> None:
    """Test run_until_done returns snapshots."""
    traj = JointTrajectory([0.0, 0.0], [1.0, 1.0], duration=0.1, dt=0.05)
    snapshots = sim.run_until_done(traj)
    assert len(snapshots) > 0
    assert snapshots[-1] == [1.0, 1.0]
    assert not sim.is_running
