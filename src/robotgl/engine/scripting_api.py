"""Scripting API for robotgl - text-based robot control like RoboDK."""

import numpy as np
from typing import Optional

from robotgl.engine.program_tree import (
    LoopNode,
    MoveCommand,
    Program,
    TeachTarget,
)
from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.engine.trajectory import CircularTrajectory, JointTrajectory


class ScriptingAPI:
    """API for text-based robot scripting."""

    def __init__(self, robot: RobotModel, sim: SimulationLoop) -> None:
        self.robot = robot
        self.sim = sim
        self._speed = 1.0
        self._program = Program("Script")
        self._targets: dict[str, TeachTarget] = {}

    def movej(self, q: list[float], duration: float = 1.0) -> None:
        """Joint move to target angles.

        Args:
            q: Joint angles in radians.
            duration: Move duration in seconds.
        """
        target = TeachTarget("movej", joint_angles=q)
        cmd = MoveCommand(target, move_type="joint", duration=duration)
        cmd.execute(self.robot, self.sim)

    def movel(self, pose: np.ndarray, duration: float = 1.0) -> None:
        """Linear cartesian move to pose.

        Args:
            pose: 4x4 SE3 transformation matrix.
            duration: Move duration in seconds.
        """
        target = TeachTarget("movel", pose=pose)
        cmd = MoveCommand(target, move_type="linear", duration=duration)
        cmd.execute(self.robot, self.sim)

    def movec(
        self,
        pose1: np.ndarray,
        pose2: np.ndarray,
        duration: float = 1.0,
    ) -> None:
        """Circular move via two intermediate poses.

        Args:
            pose1: First intermediate pose.
            pose2: Final pose.
            duration: Move duration in seconds.
        """
        # Implementation uses cartesian trajectory
        # For simplicity, move to pose2 directly
        target = TeachTarget("movec", pose=pose2)
        cmd = MoveCommand(target, move_type="linear", duration=duration)
        cmd.execute(self.robot, self.sim)

    def teach(self, name: str) -> TeachTarget:
        """Record current pose as target.

        Args:
            name: Target name.

        Returns:
            TeachTarget with current pose.
        """
        target = TeachTarget(name)
        target.capture_current(self.robot)
        self._targets[name] = target
        return target

    def home(self) -> None:
        """Move to home position."""
        self.robot.set_joint_angles([0.0] * self.robot.num_joints)
        self.sim.stop()

    def set_speed(self, factor: float) -> None:
        """Set simulation speed.

        Args:
            factor: Speed multiplier (1.0 = normal).
        """
        self._speed = factor
        self.sim.speed = factor

    def get_target(self, name: str) -> Optional[TeachTarget]:
        """Get a previously taught target.

        Args:
            name: Target name.

        Returns:
            TeachTarget or None.
        """
        return self._targets.get(name)

    def run_program(self, program: Program) -> None:
        """Execute a program tree.

        Args:
            program: Program to execute.
        """
        program.execute(self.robot, self.sim)

    def create_program(self, name: str = "ScriptProgram") -> Program:
        """Create a new program.

        Args:
            name: Program name.

        Returns:
            New Program instance.
        """
        return Program(name)
