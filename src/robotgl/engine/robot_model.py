"""RobotModel: wraps DHRobot for FK/IK in robotgl."""

from typing import Optional

import numpy as np
from roboticstoolbox import DHRobot

from robotgl.engine.dh_loader import DHLoader, RobotDef


class RobotModel:
    """Robot model using MDH convention via roboticstoolbox."""

    def __init__(self, robot_def: RobotDef) -> None:
        self._def = robot_def
        self._robot: DHRobot = DHLoader.to_dh_robot(robot_def)
        self._robot.q = [0.0] * self._robot.n  # set to zero pose

    @property
    def name(self) -> str:
        return self._def.name

    @property
    def num_joints(self) -> int:
        return self._robot.n

    @property
    def joint_limits(self) -> list[list[float]]:
        return [list(link.qlim) if link.qlim is not None else [-float("inf"), float("inf")] for link in self._robot.links]

    @property
    def current_q(self) -> list[float]:
        return list(self._robot.q)

    def fk(self, q: Optional[list[float]] = None) -> np.ndarray:
        """Forward kinematics: joint angles to SE3 pose.

        Args:
            q: Joint angles (uses current if None).

        Returns:
            4x4 SE3 homogeneous transformation matrix.
        """
        if q is not None:
            self._robot.q = q
        return self._robot.fkine(self._robot.q).A

    def ik(
        self,
        pose: np.ndarray,
        q0: Optional[list[float]] = None,
        *,
        mask: Optional[list[int]] = None,
    ) -> list[float]:
        """Inverse kinematics: SE3 pose to joint angles.

        Args:
            pose: 4x4 SE3 homogeneous transformation matrix.
            q0: Initial joint angles for IK search.
            mask: Mask for which DOF to consider (6-element list).

        Returns:
            Joint angles solving FK(q) = pose.
        """
        from spatialmath import SE3

        T = SE3(pose)
        solution = self._robot.ikine_LM(T, q0=q0, mask=mask)
        return list(solution.q)

    def set_joint_angles(self, q: list[float]) -> None:
        """Set robot to given joint angles."""
        if len(q) != self.num_joints:
            raise ValueError(f"Expected {self.num_joints} joints, got {len(q)}")
        self._robot.q = q

    @classmethod
    def from_json(cls, filepath: str) -> "RobotModel":
        """Create RobotModel from MDH JSON file.

        Args:
            filepath: Path to MDH JSON file.

        Returns:
            RobotModel instance.
        """
        robot_def = DHLoader.load(filepath)
        return cls(robot_def)
