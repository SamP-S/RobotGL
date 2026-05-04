# Plan 008: Pygame/PyOpenGL 3D Visualization

## Summary
Implement 3D robot visualization using Pygame + PyOpenGL. Render robot meshes, coordinate frames, tool paths, and workspace bounds.

## To-Do List
- [x] Create `src/robotgl/frontend/renderer.py`:
  - [x] OpenGL shaders for robot rendering
  - [x] Load mesh files (STL/OBJ) from MDH JSON
  - [x] Draw coordinate frames (X=red, Y=green, Z=blue)
  - [x] Draw tool paths (line strips)
  - [x] Draw workspace bounds (wireframe cube)
- [x] Create `src/robotgl/frontend/app.py`:
  - [x] Pygame window init with OpenGL context
  - [ ] Camera controls (orbit, zoom, pan)
  - [x] Connect to SimulationLoop callbacks for real-time updates
- [ ] Integrate Pygame_GUI for UI panels (robot control, program tree)
- [ ] Write tests (requires display, skip in headless CI)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented Renderer and RobotGLApp, imports work
