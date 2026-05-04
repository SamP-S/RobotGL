# Plan 001: Initialize Project Structure

## Summary
Set up the `robotgl` Python package with proper project structure, dependency management via `pyproject.toml`, and initial testing framework. This establishes the foundation for headless engine, frontend, and robot interfaces development.

## To-Do List
- [x] Create `.opencode/plans/` directory
- [x] Create `pyproject.toml` with:
  - [x] Package metadata (name: `robotgl`, Python 3.10+)
  - [x] Core dependencies (`roboticstoolbox-python`)
  - [x] Optional dependency groups: `[frontend]`, `[robot-interfaces]`
  - [x] Entry points (if needed later)
- [x] Create `src/robotgl/` package structure:
  - [x] `src/robotgl/__init__.py`
  - [x] `src/robotgl/engine/__init__.py`
  - [x] `src/robotgl/frontend/__init__.py`
  - [x] `src/robotgl/robot_interfaces/__init__.py`
  - [x] `src/robotgl/robot_interfaces/custom/__init__.py`
- [x] Create `tests/` directory with:
  - [x] `tests/__init__.py`
  - [x] `tests/test_engine/__init__.py` (placeholder for engine tests)
- [x] Verify local install works: `pip install .`
- [x] Verify headless install (no frontend deps): `pip install .[frontend]`
- [x] Initialize git if not already done (already initialized)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Created package structure, pyproject.toml, setup.cfg
- 2026-05-04: Verified package install and import successful
