# Plan 003: Trajectory Planning

## Summary
Implement trajectory generation for robot moves: linear (joint/cartesian), circular, and spline interpolation. Builds on RobotModel from Plan 002.

## To-Do List
- [x] Create `src/robotgl/engine/trajectory.py` with trajectory types:
  - [x] `JointTrajectory`: interpolate joint angles over time
  - [x] `CartesianTrajectory`: interpolate SE3 poses (linear in task space)
  - [x] `CircularTrajectory`: arc moves via center/radius
  - [x] `SplineTrajectory`: cubic/quintic spline interpolation
- [x] Implement trajectory sampling at fixed timesteps
- [ ] Add velocity/acceleration profiling (trapezoidal)
- [x] Write tests in `tests/test_engine/test_trajectory.py` (8/8 pass)
- [x] Verify headless execution (no display needed)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented all trajectory types, wrote tests, 8/8 pass
