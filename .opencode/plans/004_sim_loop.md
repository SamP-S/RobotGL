# Plan 004: Fixed-Timestep Simulation Loop

## Summary
Implement the headless simulation loop that runs at a fixed timestep for offline mode. Integrates RobotModel + Trajectory planning, exposes state for frontend/control.

## To-Do List
- [x] Create `src/robotgl/engine/sim_loop.py` with `SimulationLoop` class:
  - [x] Fixed timestep (configurable, default 1ms)
  - [x] State: current joint angles, target trajectory, sim time
  - [x] Methods: `start()`, `pause()`, `stop()`, `step(dt)`
  - [x] Trajectory playback with speed control
  - [x] Callback/hook for state updates (used by frontend/robot interfaces)
- [ ] Add simulation event system (reach target, error states)
- [x] Write tests in `tests/test_engine/test_sim_loop.py` (8/8 pass)
- [x] Verify headless execution

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented SimulationLoop and tests, 8/8 pass
