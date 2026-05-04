# Plan 006: Program Tree (RoboDK-Like)

## Summary
Implement visual program tree with move commands, loops, and execution engine. Supports both visual drag-and-drop and text-based Python scripting via engine API.

## To-Do List
- [x] Create `src/robotgl/engine/program_tree.py` with:
  - [x] `ProgramNode` base class
  - [x] `MoveCommand` (linear, circular, joint moves)
  - [x] `LoopNode` (repeat N times or while condition)
  - [x] `TeachTarget` (store joint/cartesian poses)
  - [x] `Program` (top-level container, executes nodes)
- [x] Implement program execution engine (walks tree, executes commands)
- [ ] Expose API for text-based Python scripting
- [x] Write tests in `tests/test_engine/test_program_tree.py` (7/7 pass)
- [x] Verify headless execution

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented ProgramNode, TeachTarget, MoveCommand, LoopNode, Program, wrote tests, 7/7 pass
