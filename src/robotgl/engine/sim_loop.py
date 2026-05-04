"""Fixed-timestep simulation loop for robotgl."""

from typing import Callable, Optional

import numpy as np

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.trajectory import Trajectory


class SimulationLoop:
    """Headless simulation loop with fixed timestep."""

    def __init__(
        self,
        robot: RobotModel,
        dt: float = 0.001,
        speed: float = 1.0,
    ) -> None:
        self.robot = robot
        self.dt = dt
        self.speed = speed
        self._current_q = robot.current_q.copy()
        self._target_q: Optional[list[float]] = None
        self._trajectory: Optional[Trajectory] = None
        self._running = False
        self._paused = False
        self._sim_time = 0.0
        self._callbacks: list[Callable] = []

    @property
    def current_q(self) -> list[float]:
        return self._current_q.copy()

    @property
    def sim_time(self) -> float:
        return self._sim_time

    @property
    def is_running(self) -> bool:
        return self._running and not self._paused

    def add_callback(self, callback: Callable[[list[float], float], None]) -> None:
        """Add callback for state updates.

        Args:
            callback: Called with (joint_angles, sim_time) on each step.
        """
        self._callbacks.append(callback)

    def start(self, trajectory: Optional[Trajectory] = None) -> None:
        """Start or resume simulation.

        Args:
            trajectory: Optional trajectory to execute.
        """
        if trajectory:
            self._trajectory = trajectory
            trajectory.reset()
        self._running = True
        self._paused = False

    def pause(self) -> None:
        """Pause simulation."""
        self._paused = True

    def stop(self) -> None:
        """Stop simulation and reset."""
        self._running = False
        self._paused = False
        self._trajectory = None
        self._sim_time = 0.0

    def step(self) -> tuple[bool, Optional[list[float]]]:
        """Execute one simulation step.

        Returns:
            (done_flag, new_joint_angles or None)
        """
        if not self._running or self._paused:
            return False, None

        effective_dt = self.dt * self.speed

        if self._trajectory:
            sample, done = self._trajectory.step()
            if isinstance(sample, np.ndarray) and sample.ndim == 2:
                # SE3 pose - extract joint angles via IK
                self._current_q = self.robot.ik(sample, q0=self._current_q)
            else:
                # Joint angles
                self._current_q = list(sample)
            self.robot.set_joint_angles(self._current_q)
            self._sim_time += effective_dt

            for callback in self._callbacks:
                callback(self._current_q, self._sim_time)

            if done:
                self._trajectory = None
                return True, self._current_q
            return False, self._current_q

        # No trajectory - just update time
        self._sim_time += effective_dt
        return False, self._current_q

    def run_until_done(self, trajectory: Trajectory) -> list[list[float]]:
        """Run simulation until trajectory completes.

        Args:
            trajectory: Trajectory to execute.

        Returns:
            List of joint angle snapshots.
        """
        self.start(trajectory)
        snapshots = []
        while True:
            done, q = self.step()
            if q is not None:
                snapshots.append(q)
            if done:
                break
        self.stop()
        return snapshots
