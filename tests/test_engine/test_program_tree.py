"""Tests for Program Tree (headless)."""

import numpy as np
import pytest

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.engine.program_tree import (
    Program,
    MoveCommand,
    LoopNode,
    TeachTarget,
)


@pytest.fixture
def robot_and_sim() -> tuple[RobotModel, SimulationLoop]:
    """Create robot and simulation."""
    robot = RobotModel.from_json("tests/test_data/2dof_md.json")
    sim = SimulationLoop(robot, dt=0.001, speed=100.0)
    return robot, sim


def test_teach_target_capture(robot_and_sim: tuple) -> None:
    """Test teaching captures current pose."""
    robot, _ = robot_and_sim
    target = TeachTarget(name="Home")
    target.capture_current(robot)
    assert target.joint_angles == [0.0, 0.0]
    assert target.pose is not None
    assert target.pose.shape == (4, 4)


def test_teach_target_move(robot_and_sim: tuple) -> None:
    """Test moving to a taught target."""
    robot, sim = robot_and_sim
    target = TeachTarget(name="Target1")
    target.joint_angles = [0.5, 0.5]
    cmd = MoveCommand(target, move_type="joint", duration=0.1)
    cmd.execute(robot, sim)
    assert np.allclose(robot.current_q, [0.5, 0.5], atol=0.05)


def test_loop_node_count(robot_and_sim: tuple) -> None:
    """Test loop executes N times."""
    robot, sim = robot_and_sim
    target1 = TeachTarget("T1")
    target1.joint_angles = [0.3, 0.3]
    target2 = TeachTarget("T2")
    target2.joint_angles = [0.0, 0.0]

    loop = LoopNode(count=2, name="Repeat2x")
    cmd1 = MoveCommand(target1, duration=0.1)
    cmd2 = MoveCommand(target2, duration=0.1)
    loop.add_child(cmd1)
    loop.add_child(cmd2)

    program = Program("Test")
    program.add_child(loop)
    program.execute(robot, sim)

    assert np.allclose(robot.current_q, [0.0, 0.0], atol=0.05)


def test_program_tree_structure(robot_and_sim: tuple) -> None:
    """Test program tree hierarchy."""
    program = Program("Main")
    loop = LoopNode(count=1, name="Repeat1")
    target = TeachTarget("Target")
    cmd = MoveCommand(target)
    cmd.add_child(target)  # TeachTarget is a child of MoveCommand

    loop.add_child(cmd)
    program.add_child(loop)

    nodes = program.traverse()
    assert len(nodes) == 4  # Program + Loop + MoveCommand + TeachTarget
    assert nodes[0].name == "Main"
    assert nodes[1].name == "Repeat1"


def test_program_stop(robot_and_sim: tuple) -> None:
    """Test stopping program execution."""
    robot, sim = robot_and_sim
    program = Program("Test")

    target1 = TeachTarget("T1")
    target1.joint_angles = [0.5, 0.5]
    target2 = TeachTarget("T2")
    target2.joint_angles = [1.0, 1.0]

    cmd1 = MoveCommand(target1, duration=0.5)
    cmd2 = MoveCommand(target2, duration=0.5)
    program.add_child(cmd1)
    program.add_child(cmd2)

    # Execute and stop quickly
    import threading
    thread = threading.Thread(target=program.execute, args=(robot, sim))
    thread.start()
    program.stop()  # Stop while executing
    thread.join(timeout=2.0)
    # Should have started moving but stopped before reaching target
    assert robot.current_q[0] < 0.5  # Not yet at target
    assert not program.is_executing


def test_move_linear(robot_and_sim: tuple) -> None:
    """Test linear move command."""
    robot, sim = robot_and_sim
    target = TeachTarget("LinearTarget")
    target.pose = robot.fk([0.5, 0.5])
    target.joint_angles = [0.5, 0.5]

    cmd = MoveCommand(target, move_type="linear", duration=0.1)
    cmd.execute(robot, sim)
    assert np.allclose(robot.current_q, [0.5, 0.5], atol=0.05)


def test_node_parent_child(robot_and_sim: tuple) -> None:
    """Test parent-child relationships."""
    program = Program("Main")
    loop = LoopNode(count=1)
    cmd = MoveCommand(TeachTarget("T"))

    program.add_child(loop)
    loop.add_child(cmd)

    assert cmd.parent == loop
    assert loop.parent == program
    assert program.parent is None
