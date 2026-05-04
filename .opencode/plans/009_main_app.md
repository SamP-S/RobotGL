# Plan 009: Main Application Integration

## Summary
Tie all components together: engine, frontend, robot interfaces. Create main entry point that launches the app, loads robot JSON, and connects UI to simulation.

## To-Do List
- [x] Create `src/robotgl/main.py`:
  - [x] Parse CLI arguments (robot JSON, headless mode, interface type)
  - [x] Load robot model from JSON
  - [x] Initialize SimulationLoop
  - [x] Initialize frontend (or run headless)
  - [x] Connect robot interface (serial/TCP/custom)
  - [x] Main loop: sync simulation <-> interface
- [x] Add `pyproject.toml` entry point: `robotgl = "robotgl.main:main"`
- [x] Write integration tests (6/6 pass)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Main app created, entry point configured, 6/6 tests pass
