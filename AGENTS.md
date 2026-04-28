# AGENTS.md

This is a combined python & arduino project, primarily python, for robot visualisation/simulation. Tasks should only cover one part of the project at a time.

## Core Rules
- Always ask if anything is unclear, incomplete or has possible issues.
- Keep answers and questions short and consise, if further detail is required the user will let you know.
- You are not allowed to do anything outside of this project directory
- You should ask before executing any bash

## Planning
- Plans should always be written before any code changes are made
- Plans are always written in ".opencode/plans"
- Plans should be labelled sequentially with the filenames using underscores i.e. "003_feat-seperate-app-layers.md"
- User may refer to plans by index number i.e. in plan 3
- Plans should always have a summary and have a checkbox To-Do list so agents can start and stop working on projects when necessary
- You should keep a shorthanded history of changes made in each plan as you got to keep track of code changes

## Code Style

### Python
- Use modern Python 3.10+
- Type hints on all functions
- Short functions, clear names
- Add docstrings for public functions
- Use snake_case for variables and functions and PascalCase for classes
- Minimise package imports, prioritise standard packages over community packages
- Private variables and functions should have a prefix underscore "_my_var"
- Strings should use double quotation marks ""

## Testing
- Use "pytest"
- Place tests in "tests/"
- Run: pytest

## When Suggesting Changes
- Output relevant file content when editing
- Use triple backticks with language specicfier
- Explain reasoning before code
