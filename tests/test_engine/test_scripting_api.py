"""Tests for Scripting API (headless)."""

import numpy as np
import pytest

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.engine.scripting_api import ScriptingAPI


@pytest.fixture
def api() -> ScriptingAPI:
    """Create scripting API with 2DOF robot."""
    robot = RobotModel.from_json("tests/test_data/2dof_md.json")
    sim = SimulationLoop(robot, dt=0.001, speed=100.0)
    return ScriptingAPI(robot, sim)


def test_movej(api: ScriptingAPI) -> None:
    """Test joint move."""
    api.movej([0.5, 0.5], duration=0.1)
    assert np.allclose(api.robot.current_q, [0.5, 0.5], atol=0.05)


def test_movel(api: ScriptingAPI) -> None:
    """Test linear cartesian move."""
    target_pose = api.robot.fk([0.3, 0.3])
    api.movel(target_pose, duration=0.1)
    assert np.allclose(api.robot.current_q, [0.3, 0.3], atol=0.05)


def test_teach(api: ScriptingAPI) -> None:
    """Test teach target."""
    api.robot.set_joint_angles([0.2, 0.4])
    target = api.teach("Target1")
    assert target.joint_angles == [0.2, 0.4]
    assert "Target1" in api._targets


def test_home(api: ScriptingAPI) -> None:
    """Test home command."""
    api.robot.set_joint_angles([0.5, 0.5])
    api.home()
    assert np.allclose(api.robot.current_q, [0.0, 0.0], atol=0.01)


def test_set_speed(api: ScriptingAPI) -> None:
    """Test speed setting."""
    api.set_speed(2.0)
    assert api._speed == 2.0
    assert api.sim.speed == 2.0


def test_get_target(api: ScriptingAPI) -> None:
    """Test getting taught target."""
    target = api.teach("MyTarget")
    retrieved = api.get_target("MyTarget")
    assert retrieved is target
    assert api.get_target("NonExistent") is None


def test_create_program(api: ScriptingAPI) -> None:
    """Test creating program."""
    program = api.create_program("TestProg")
    assert program.name == "TestProg"
    from robotgl.engine.program_tree import Program
    assert isinstance(program, Program)


def test_run_program(api: ScriptingAPI) -> None:
    """Test running a program."""
    from robotgl.engine.program_tree import MoveCommand, TeachTarget

    program = api.create_program("MoveProg")
    target = TeachTarget("T1")
    target.joint_angles = [0.3, 0.3]
    cmd = MoveCommand(target, duration=0.1)
    program.add_child(cmd)
    api.run_program(program)
    assert np.allclose(api.robot.current_q, [0.3, 0.3], atol=0.05)
