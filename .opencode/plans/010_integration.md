# Plan 010: Integration and Camera Controls

## Summary
Add camera controls to frontend (orbit, zoom, pan) and write integration tests connecting all components.

## To-Do List
- [ ] Add camera controls to `src/robotgl/frontend/app.py`:
  - [ ] Orbit (mouse drag left button)
  - [ ] Zoom (mouse wheel)
  - [ ] Pan (mouse drag right button)
- [ ] Add Pygame_GUI panels:
  - [ ] Robot control panel (home, stop, speed slider)
  - [ ] Program tree panel (list of moves/loops)
  - [ ] Joint angle display
- [ ] Write integration test: robot -> sim -> frontend -> interface
- [ ] Update AGENTS.md with all implemented features

## Change History
- 2026-05-04: Plan created
