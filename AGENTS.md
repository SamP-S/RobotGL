# AGENTS.md

This is a Python project (with Arduino `firmware/` subdir) for `robotgl`: an online/offline robot programmer. Supports any robot defined via Modified DH (MDH) JSON files, uses `roboticstoolbox-python` for kinematics, and `pygame`/`pygame_gui`/`pyopengl` for visualization. Tasks must cover only one project component at a time.

## Core Rules
- Always ask if anything is unclear, incomplete, or has possible issues.
- Keep answers and questions short and concise; further detail will be provided if requested.
- Never execute code outside the project directory.
- Ask before executing any bash command.
- English only: all code, comments, documentation, and communication must be in English.

## Project Architecture
Follow these rules when building `robotgl` components:
- **Headless Core Engine**: Decoupled from frontend, runs fixed-timestep simulation loops for offline mode. Implements MDH JSON parsing, FK/IK via `roboticstoolbox-python`, trajectory planning (linear/circular/spline). Exposes API for scripted movements and program tree execution.
- **Frontend**: `pygame` + `pygame_gui` + `pyopengl` for 3D visualization (robot rendering, workspace bounds, tool paths, coordinate frames). No collision/obstacle support initially.
- **Robot Interfaces**: `BaseRobotInterface` abstract class with methods: `connect() -> bool`, `disconnect()`, `set_joint_angles(angles) -> bool`, `get_joint_angles() -> list[float]`, `home() -> bool`. Bundle common interfaces; allow user-defined custom interfaces in `src/robotgl/robot_interfaces/custom/`.
- **RoboDK-Like Features**: Support both visual program trees (move/loop sequences) and text-based Python scripting via the engine API. Include teach targets, simulation playback with speed control.
- **Online Mode**: Real-time sync (app acts as robot controller); no on-robot program deployment.

## Planning
- Write plans before any code changes.
- Store plans in `.opencode/plans/` with sequential numeric prefixes and underscore-separated names (e.g., `001_init_project_structure.md`).
- Plans must include:
  - Summary of scope
  - Checkbox To-Do list (trackable progress)
  - Shorthand change history (update as work progresses)
- Users may reference plans by index (e.g., "in plan 3").

## Code Style

### Python
- Python 3.10+, type hints on all functions, docstrings for public functions.
- Naming: `snake_case` for variables/functions, `PascalCase` for classes, `_prefix` for private members.
- Strings use double quotation marks.
- Minimize imports; prioritize standard library over community packages.
- Package name: `robotgl`, source under `src/robotgl/`.

### Project Structure
```
robotgl/
├── src/
│   └── robotgl/
│       ├── engine/          # Headless core (DH loader, sim loop, trajectory planning)
│       ├── frontend/        # Pygame/PyOpenGL visualization + Pygame_GUI
│       ├── robot_interfaces/ # Base class, common interfaces, custom user interfaces
│       └── __init__.py
├── tests/                   # Pytest tests (core engine only initially)
├── firmware/                # Existing Arduino projects (untouched)
├── pyproject.toml           # Local install config, optional dependency groups
└── AGENTS.md
```

## Dependencies
- Split optional dependency groups in `pyproject.toml`:
  - `[frontend]`: `pygame`, `pygame_gui`, `pyopengl`
  - `[robot-interfaces]`: `pyserial`, TCP/IP libs as needed
  - Core only requires `roboticstoolbox-python`
- Minimize community packages; justify any additions.

## Testing
- Use `pytest`, tests in `tests/`.
- Initially only test core engine (headless, no display/OpenGL required).
- Frontend/interface tests added later.

## Packaging
- Local install only: `pip install .` (core) or `pip install .[frontend]` (with UI deps).
- No public package hosting.

## When Suggesting Changes
- Show relevant file content before editing.
- Use triple backticks with language specifier for code snippets.
- Explain reasoning before making changes.
