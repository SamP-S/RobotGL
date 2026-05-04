# Plan 007: Scripting API

## Summary
Expose Python scripting API for text-based robot control. Users can write Python scripts that call engine functions (move, teach, etc.) similar to RoboDK's API.

## To-Do List
- [x] Create `src/robotgl/engine/scripting_api.py` with:
  - [x] `movej(q, speed=1.0)` - Joint move
  - [x] `movel(pose, speed=1.0)` - Linear cartesian move
  - [x] `movec(pose1, pose2, speed=1.0)` - Circular move
  - [x] `teach()` - Record current pose as target
  - [x] `home()` - Move to home
  - [x] `set_speed(factor)` - Set simulation speed
- [x] Provide script execution environment (restricted globals)
- [x] Write tests in `tests/test_engine/test_scripting_api.py` (8/8 pass)
- [x] Add `create_program()` and `run_program()` methods

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented ScriptingAPI, wrote tests, 8/8 pass
