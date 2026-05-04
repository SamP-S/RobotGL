# Plan 002: MDH JSON Loader + RobotModel

## Summary
Implement Modified DH (MDH) JSON parsing, `RobotModel` class wrapping `roboticstoolbox-python` for FK/IK, and headless tests. This is the foundation for all downstream engine features.

## To-Do List
- [x] Define MDH JSON schema (required: per-link MDH params, optional: joint limits/mass/meshes)
- [x] Create `src/robotgl/engine/dh_loader.py` to parse MDH JSON files
- [x] Create `src/robotgl/engine/robot_model.py` with `RobotModel` class:
  - [x] Load MDH params via `DHLoader`
  - [x] Wrap `roboticstoolbox.DHRobot` (MDH mode)
  - [x] Implement `fk(q: list[float]) -> np.ndarray` (SE3 pose)
  - [x] Implement `ik(pose: np.ndarray) -> list[float]` (joint angles)
  - [x] Expose joint limits, link metadata
- [x] Update `src/robotgl/engine/__init__.py` to export new classes
- [x] Create sample MDH JSON test file (`tests/test_data/2dof_md.json`)
- [x] Write headless tests in `tests/test_engine/test_robot_model.py`
- [x] Run `pytest` to verify FK/IK works (7/7 tests pass)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented DHLoader, RobotModel, wrote tests, all 7 tests pass
