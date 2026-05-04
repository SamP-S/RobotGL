"""Main Pygame application for robotgl."""

import sys
from typing import Optional

import pygame
from pygame.locals import *

from robotgl.engine.robot_model import RobotModel
from robotgl.engine.sim_loop import SimulationLoop
from robotgl.frontend.renderer import Renderer


class RobotGLApp:
    """Main application tying frontend to engine."""

    def __init__(
        self,
        robot_json: str,
        width: int = 800,
        height: int = 600,
    ) -> None:
        self.robot = RobotModel.from_json(robot_json)
        self.sim = SimulationLoop(self.robot, dt=0.001, speed=1.0)
        self.renderer = Renderer(width, height)
        self._running = False
        self._clock = pygame.time.Clock()
        self._tool_path: list = []

    def run(self) -> None:
        """Run the application."""
        pygame.init()
        pygame.display.set_mode(
            (self.renderer.width, self.renderer.height),
            pygame.OPENGL | pygame.DOUBLEBUF,
        )
        pygame.display.set_caption("robotgl")

        # Callback to track tool path
        self.sim.add_callback(self._on_sim_step)

        self._running = True
        while self._running:
            self._handle_events()
            self._render()
            self._clock.tick(60)

        pygame.quit()
        sys.exit()

    def _handle_events(self) -> None:
        """Handle pygame events."""
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self._running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    self._running = False
                elif event.key == pygame.K_SPACE:
                    if self.sim.is_running:
                        self.sim.pause()
                    else:
                        self.sim.start()
                elif event.key == pygame.K_h:
                    self.robot.home()
                    self.sim.stop()

    def _render(self) -> None:
        """Render one frame."""
        self.renderer.clear()
        self.renderer.setup_camera()
        self.renderer.draw_workspace_bounds()
        self.renderer.draw_robot(self.robot.current_q)
        self.renderer.draw_coordinate_frame(scale=0.5)
        if self._tool_path:
            self.renderer.draw_tool_path(self._tool_path)
        pygame.display.flip()

    def _on_sim_step(self, q: list[float], t: float) -> None:
        """Callback on simulation step."""
        pose = self.robot.fk(q)
        self._tool_path.append(pose[:3, 3])
        # Limit path length
        if len(self._tool_path) > 1000:
            self._tool_path.pop(0)


def main() -> None:
    """Entry point for robotgl."""
    import argparse

    parser = argparse.ArgumentParser(description="robotgl - Robot Programmer")
    parser.add_argument("robot", help="Path to robot MDH JSON file")
    args = parser.parse_args()

    app = RobotGLApp(args.robot)
    app.run()


if __name__ == "__main__":
    main()
