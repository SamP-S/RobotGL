"""Program tree for robotgl - RoboDK-like visual programming."""

from abc import ABC, abstractmethod
from typing import Any, Callable, Optional

import numpy as np

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.trajectory import (
    CircularTrajectory,
    JointTrajectory,
    Trajectory,
)


class ProgramNode(ABC):
    """Base class for all program nodes."""

    def __init__(self, name: str = "") -> None:
        self.name = name
        self.children: list["ProgramNode"] = []
        self.parent: Optional["ProgramNode"] = None

    def add_child(self, node: "ProgramNode") -> None:
        """Add a child node."""
        node.parent = self
        self.children.append(node)

    @abstractmethod
    def execute(self, robot: RobotModel, sim: Any) -> None:
        """Execute this node.

        Args:
            robot: RobotModel instance
            sim: SimulationLoop instance
        """
        pass

    def traverse(self) -> list["ProgramNode"]:
        """Traverse tree depth-first."""
        result = [self]
        for child in self.children:
            result.extend(child.traverse())
        return result


class TeachTarget(ProgramNode):
    """Store a target pose (joint or cartesian)."""

    def __init__(
        self,
        name: str = "",
        joint_angles: Optional[list[float]] = None,
        pose: Optional[np.ndarray] = None,
    ) -> None:
        super().__init__(name)
        self.joint_angles = joint_angles
        self.pose = pose

    def capture_current(self, robot: RobotModel) -> None:
        """Capture current robot pose."""
        self.joint_angles = robot.current_q
        self.pose = robot.fk()

    def execute(self, robot: RobotModel, sim: Any) -> None:
        """Move to this target."""
        if self.joint_angles:
            traj = JointTrajectory(
                robot.current_q, self.joint_angles, duration=1.0
            )
            sim.start(traj)
            while not sim.step()[0]:
                pass


class MoveCommand(ProgramNode):
    """Move command (joint, linear, or circular)."""

    def __init__(
        self,
        target: TeachTarget,
        move_type: str = "joint",
        duration: float = 1.0,
        name: str = "",
    ) -> None:
        super().__init__(name)
        self.target = target
        self.move_type = move_type
        self.duration = duration

    def execute(self, robot: RobotModel, sim: Any) -> None:
        """Execute move command."""
        if self.move_type == "joint":
            if self.target.joint_angles:
                traj = JointTrajectory(
                    robot.current_q, self.target.joint_angles, self.duration
                )
            else:
                return
        elif self.move_type == "linear":
            if self.target.pose is not None:
                traj = JointTrajectory(
                    robot.current_q,
                    robot.ik(self.target.pose),
                    self.duration,
                )
            else:
                return
        else:
            raise ValueError(f"Unknown move type: {self.move_type}")

        sim.start(traj)
        while True:
            done, _ = sim.step()
            # Check if parent program wants to stop
            program = self._find_program()
            if program and not program.is_executing:
                break
            if done:
                break

    def _find_program(self) -> Optional["Program"]:
        """Find parent Program by traversing up the tree."""
        node: Optional[ProgramNode] = self
        while node:
            if isinstance(node, Program):
                return node
            node = node.parent
        return None


class LoopNode(ProgramNode):
    """Loop over child nodes N times or while condition."""

    def __init__(
        self,
        count: int = 1,
        condition: Optional[Callable[[], bool]] = None,
        name: str = "",
    ) -> None:
        super().__init__(name)
        self.count = count
        self.condition = condition

    def execute(self, robot: RobotModel, sim: Any) -> None:
        """Execute children in loop."""
        if self.condition:
            while self.condition():
                for child in self.children:
                    child.execute(robot, sim)
        else:
            for _ in range(self.count):
                for child in self.children:
                    child.execute(robot, sim)


class Program(ProgramNode):
    """Top-level program container."""

    def __init__(self, name: str = "Main Program") -> None:
        super().__init__(name)
        self._executing = False

    def execute(self, robot: RobotModel, sim: Any) -> None:
        """Execute entire program."""
        self._executing = True
        for child in self.children:
            if not self._executing:
                break
            child.execute(robot, sim)
        self._executing = False

    def stop(self) -> None:
        """Stop program execution."""
        self._executing = False

    @property
    def is_executing(self) -> bool:
        return self._executing
