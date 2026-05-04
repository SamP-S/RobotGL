# Plan 005: Robot Interfaces

## Summary
Implement `BaseRobotInterface` abstract class and common robot communication interfaces (serial, TCP/IP). Allow user-defined custom interfaces.

## To-Do List
- [x] Create `src/robotgl/robot_interfaces/base.py` with `BaseRobotInterface` ABC:
  - [x] `connect() -> bool`
  - [x] `disconnect()`
  - [x] `set_joint_angles(angles) -> bool`
  - [x] `get_joint_angles() -> list[float]`
  - [x] `home() -> bool`
- [x] Implement common interfaces:
  - [x] `SerialInterface` (pyserial-based)
  - [x] `TcpInterface` (socket-based)
- [x] Create `custom/` folder for user-defined interfaces
- [ ] Add interface factory/registry for dynamic loading
- [x] Write tests in `tests/test_robot_interfaces/` (6/6 pass)

## Change History
- 2026-05-04: Plan created
- 2026-05-04: Implemented BaseRobotInterface, SerialInterface, TcpInterface, wrote tests, 6/6 pass
