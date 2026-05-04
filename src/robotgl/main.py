"""Main entry point for robotgl - ties all components together."""

import argparse
import sys
from typing import Optional

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.robot_interfaces.base import BaseRobotInterface
from robotgl.robot_interfaces.serial_interface import SerialInterface
from robotgl.robot_interfaces.tcp_interface import TcpInterface


def create_robot_interface(
    interface_type: str,
    **kwargs,
) -> Optional[BaseRobotInterface]:
    """Factory function to create robot interface.

    Args:
        interface_type: "serial", "tcp", or "none"
        **kwargs: Arguments for interface constructor.

    Returns:
        BaseRobotInterface instance or None.
    """
    if interface_type == "serial":
        return SerialInterface(**kwargs)
    elif interface_type == "tcp":
        return TcpInterface(**kwargs)
    elif interface_type == "none":
        return None
    else:
        raise ValueError(f"Unknown interface type: {interface_type}")


def run_headless(
    robot: RobotModel,
    sim: SimulationLoop,
    interface: Optional[BaseRobotInterface],
) -> None:
    """Run in headless mode (no display).

    Args:
        robot: RobotModel instance.
        sim: SimulationLoop instance.
        interface: Optional robot interface for online mode.
    """
    print(f"Loaded robot: {robot.name}")
    print(f"Joints: {robot.num_joints}")
    print(f"Current pose: {robot.current_q}")

    if interface:
        print(f"Connecting to robot...")
        if interface.connect():
            print("Connected successfully")
            # Sync robot state
            interface.set_joint_angles(robot.current_q)
        else:
            print("Connection failed")
            return

    print("Running headless simulation...")
    print("Press Ctrl+C to stop")

    try:
        while True:
            done, q = sim.step()
            if q and interface:
                interface.set_joint_angles(q)
            if done:
                sim.stop()
    except KeyboardInterrupt:
        print("\nStopped")
    finally:
        if interface:
            interface.disconnect()


def run_frontend(
    robot_json: str,
    interface: Optional[BaseRobotInterface],
) -> None:
    """Run with Pygame frontend.

    Args:
        robot_json: Path to robot MDH JSON file.
        interface: Optional robot interface.
    """
    from robotgl.frontend.app import RobotGLApp

    app = RobotGLApp(robot_json)
    if interface:
        # Store interface in app for later use
        app.robot_interface = interface
    app.run()


def main() -> None:
    """Parse CLI arguments and launch application."""
    parser = argparse.ArgumentParser(
        description="robotgl - Online/Offline Robot Programmer"
    )
    parser.add_argument(
        "robot",
        help="Path to robot MDH JSON file",
    )
    parser.add_argument(
        "--headless",
        action="store_true",
        help="Run without GUI (headless mode)",
    )
    parser.add_argument(
        "--interface",
        choices=["none", "serial", "tcp"],
        default="none",
        help="Robot interface type (default: none)",
    )
    parser.add_argument(
        "--port",
        help="Serial port (for serial interface)",
    )
    parser.add_argument(
        "--host",
        default="localhost",
        help="TCP host (for TCP interface)",
    )
    parser.add_argument(
        "--tcp-port",
        type=int,
        default=8080,
        help="TCP port (for TCP interface)",
    )

    args = parser.parse_args()

    # Load robot
    try:
        robot = RobotModel.from_json(args.robot)
    except Exception as e:
        print(f"Error loading robot: {e}")
        sys.exit(1)

    # Create simulation loop
    sim = SimulationLoop(robot, dt=0.001, speed=1.0)

    # Create robot interface
    interface: Optional[BaseRobotInterface] = None
    try:
        if args.interface == "serial":
            if not args.port:
                print("Error: --port required for serial interface")
                sys.exit(1)
            interface = create_robot_interface("serial", port=args.port)
        elif args.interface == "tcp":
            interface = create_robot_interface(
                "tcp", host=args.host, port=args.tcp_port
            )
    except Exception as e:
        print(f"Error creating interface: {e}")
        sys.exit(1)

    # Run
    if args.headless:
        run_headless(robot, sim, interface)
    else:
        run_frontend(args.robot, interface)


if __name__ == "__main__":
    main()
