"""Trajectory generation for robotgl."""

from abc import ABC, abstractmethod
from typing import Optional

import numpy as np


class Trajectory(ABC):
    """Base class for all trajectories."""

    def __init__(self, duration: float, dt: float = 0.001) -> None:
        self.duration = duration
        self.dt = dt
        self._current_time = 0.0

    @abstractmethod
    def sample(self, t: Optional[float] = None) -> np.ndarray:
        """Sample trajectory at time t.

        Args:
            t: Time in seconds. Uses internal time if None.

        Returns:
            Joint angles or SE3 pose depending on trajectory type.
        """
        pass

    def reset(self) -> None:
        """Reset trajectory to start."""
        self._current_time = 0.0

    def step(self) -> tuple[np.ndarray, bool]:
        """Step forward by dt.

        Returns:
            (sample at new time, done flag)
        """
        self._current_time = min(self._current_time + self.dt, self.duration)
        sample = self.sample(self._current_time)
        done = self._current_time >= self.duration
        return sample, done


class JointTrajectory(Trajectory):
    """Interpolate joint angles over time."""

    def __init__(
        self,
        start_q: list[float],
        end_q: list[float],
        duration: float,
        dt: float = 0.001,
    ) -> None:
        super().__init__(duration, dt)
        self.start_q = np.array(start_q, dtype=float)
        self.end_q = np.array(end_q, dtype=float)
        self.num_joints = len(start_q)
        if len(end_q) != self.num_joints:
            raise ValueError("start_q and end_q must have same length")

    def sample(self, t: Optional[float] = None) -> np.ndarray:
        """Linear interpolation of joint angles."""
        if t is None:
            t = self._current_time
        t = np.clip(t / self.duration, 0.0, 1.0)
        return self.start_q + t * (self.end_q - self.start_q)


class CartesianTrajectory(Trajectory):
    """Interpolate SE3 poses linearly in task space."""

    def __init__(
        self,
        start_pose: np.ndarray,
        end_pose: np.ndarray,
        duration: float,
        dt: float = 0.001,
    ) -> None:
        super().__init__(duration, dt)
        self.start_pose = start_pose
        self.end_pose = end_pose

    def sample(self, t: Optional[float] = None) -> np.ndarray:
        """Linear interpolation of SE3 pose (position only)."""
        if t is None:
            t = self._current_time
        s = np.clip(t / self.duration, 0.0, 1.0)
        pose = self.start_pose.copy()
        pose[:3, 3] = self.start_pose[:3, 3] + s * (self.end_pose[:3, 3] - self.start_pose[:3, 3])
        return pose


class CircularTrajectory(Trajectory):
    """Circular arc move in task space."""

    def __init__(
        self,
        center: np.ndarray,
        radius: float,
        start_angle: float,
        end_angle: float,
        plane: str = "xy",
        duration: float = 1.0,
        dt: float = 0.001,
    ) -> None:
        super().__init__(duration, dt)
        self.center = center
        self.radius = radius
        self.start_angle = start_angle
        self.end_angle = end_angle
        self.plane = plane

    def sample(self, t: Optional[float] = None) -> np.ndarray:
        """Sample point on circular arc."""
        if t is None:
            t = self._current_time
        s = np.clip(t / self.duration, 0.0, 1.0)
        angle = self.start_angle + s * (self.end_angle - self.start_angle)
        pose = np.eye(4)
        if self.plane == "xy":
            pose[0, 3] = self.center[0] + self.radius * np.cos(angle)
            pose[1, 3] = self.center[1] + self.radius * np.sin(angle)
            pose[2, 3] = self.center[2]
        return pose


class SplineTrajectory(Trajectory):
    """Cubic spline interpolation for smooth motion."""

    def __init__(
        self,
        waypoints: list[np.ndarray],
        durations: list[float],
        dt: float = 0.001,
    ) -> None:
        super().__init__(sum(durations), dt)
        self.waypoints = waypoints
        self.segment_durations = durations
        self.num_segments = len(waypoints) - 1
        self._compute_coefficients()

    def _compute_coefficients(self) -> None:
        """Compute cubic spline coefficients for each segment."""
        n = self.num_segments
        self.coeffs = []
        for i in range(n):
            p0 = self.waypoints[i]
            p1 = self.waypoints[i + 1]
            dur = self.segment_durations[i]
            self.coeffs.append((p0, p1 - p0, dur))

    def sample(self, t: Optional[float] = None) -> np.ndarray:
        """Sample cubic spline at time t."""
        if t is None:
            t = self._current_time
        t = np.clip(t, 0.0, self.duration)
        elapsed = t
        for i, dur in enumerate(self.segment_durations):
            if elapsed <= dur or i == self.num_segments - 1:
                s = np.clip(elapsed / dur, 0.0, 1.0)
                p0, diff, _ = self.coeffs[i]
                return p0 + diff * (3 * s ** 2 - 2 * s ** 3)
            elapsed -= dur
        return self.waypoints[-1]
