"""Pygame/PyOpenGL 3D renderer for robotgl."""

import numpy as np
from typing import Optional

from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *


class Renderer:
    """3D renderer using PyOpenGL."""

    def __init__(self, width: int = 800, height: int = 600) -> None:
        self.width = width
        self.height = height
        self._init_gl()

    def _init_gl(self) -> None:
        """Initialize OpenGL state."""
        glClearColor(0.1, 0.1, 0.1, 1.0)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glLight(GL_LIGHT0, GL_POSITION, (10.0, 10.0, 10.0, 1.0))
        glLight(GL_LIGHT0, GL_DIFFUSE, (1.0, 1.0, 1.0, 1.0))

    def setup_camera(
        self,
        eye: tuple[float, float, float] = (5.0, 5.0, 5.0),
        target: tuple[float, float, float] = (0.0, 0.0, 0.0),
    ) -> None:
        """Setup perspective and camera."""
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, self.width / self.height, 0.1, 100.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(*eye, *target, 0.0, 0.0, 1.0)

    def clear(self) -> None:
        """Clear screen and depth buffer."""
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    def draw_robot(
        self,
        joint_angles: list[float],
        link_lengths: Optional[list[float]] = None,
    ) -> None:
        """Draw simple robot (lines for links).

        Args:
            joint_angles: Current joint angles.
            link_lengths: Length of each link (defaults to 0.5 per link).
        """
        if link_lengths is None:
            link_lengths = [0.5] * len(joint_angles)

        glPushMatrix()
        glColor3f(0.8, 0.8, 0.8)  # Light gray robot

        # Base
        glPushMatrix()
        glTranslatef(0.0, 0.0, -0.1)
        self._draw_cube(0.4, 0.4, 0.2)
        glPopMatrix()

        # Links
        for i, (angle, length) in enumerate(zip(joint_angles, link_lengths)):
            glRotatef(np.degrees(angle), 0.0, 0.0, 1.0)
            self._draw_cylinder(0.1, length)
            glTranslatef(length, 0.0, 0.0)
            self._draw_joint()

        glPopMatrix()

    def draw_coordinate_frame(
        self,
        pose: Optional[np.ndarray] = None,
        scale: float = 0.3,
    ) -> None:
        """Draw RGB coordinate frame (X=red, Y=green, Z=blue)."""
        glPushMatrix()
        if pose is not None:
            glMultMatrixf(pose.T.flatten())

        # X axis (red)
        glColor3f(1.0, 0.0, 0.0)
        self._draw_arrow(scale, (1.0, 0.0, 0.0))

        # Y axis (green)
        glColor3f(0.0, 1.0, 0.0)
        self._draw_arrow(scale, (0.0, 1.0, 0.0))

        # Z axis (blue)
        glColor3f(0.0, 0.0, 1.0)
        self._draw_arrow(scale, (0.0, 0.0, 1.0))

        glPopMatrix()

    def draw_workspace_bounds(
        self,
        min_xyz: tuple[float, float, float] = (-1.0, -1.0, 0.0),
        max_xyz: tuple[float, float, float] = (1.0, 1.0, 1.0),
    ) -> None:
        """Draw workspace bounds as wireframe cube."""
        glPushMatrix()
        glColor3f(0.3, 0.3, 0.3)  # Dark gray
        glTranslatef(
            (min_xyz[0] + max_xyz[0]) / 2,
            (min_xyz[1] + max_xyz[1]) / 2,
            (min_xyz[2] + max_xyz[2]) / 2,
        )
        glScale(
            max_xyz[0] - min_xyz[0],
            max_xyz[1] - min_xyz[1],
            max_xyz[2] - min_xyz[2],
        )
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        self._draw_cube(1.0, 1.0, 1.0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        glPopMatrix()

    def draw_tool_path(self, points: list[np.ndarray]) -> None:
        """Draw tool path as line strip."""
        if len(points) < 2:
            return
        glPushMatrix()
        glColor3f(1.0, 1.0, 0.0)  # Yellow path
        glBegin(GL_LINE_STRIP)
        for p in points:
            glVertex3f(p[0], p[1], p[2])
        glEnd()
        glPopMatrix()

    def _draw_cube(self, w: float, h: float, d: float) -> None:
        """Draw wireframe cube."""
        glBegin(GL_QUADS)
        # Front
        glVertex3f(-w, -h, -d)
        glVertex3f(w, -h, -d)
        glVertex3f(w, h, -d)
        glVertex3f(-w, h, -d)
        # Back
        glVertex3f(-w, -h, d)
        glVertex3f(w, -h, d)
        glVertex3f(w, h, d)
        glVertex3f(-w, h, d)
        # Top
        glVertex3f(-w, h, -d)
        glVertex3f(w, h, -d)
        glVertex3f(w, h, d)
        glVertex3f(-w, h, d)
        # Bottom
        glVertex3f(-w, -h, -d)
        glVertex3f(w, -h, -d)
        glVertex3f(w, -h, d)
        glVertex3f(-w, -h, d)
        # Right
        glVertex3f(w, -h, -d)
        glVertex3f(w, h, -d)
        glVertex3f(w, h, d)
        glVertex3f(w, -h, d)
        # Left
        glVertex3f(-w, -h, -d)
        glVertex3f(-w, h, -d)
        glVertex3f(-w, h, d)
        glVertex3f(-w, -h, d)
        glEnd()

    def _draw_cylinder(self, radius: float, length: float, slices: int = 16) -> None:
        """Draw cylinder along X axis."""
        glBegin(GL_QUAD_STRIP)
        for i in range(slices + 1):
            angle = 2.0 * np.pi * i / slices
            y = radius * np.cos(angle)
            z = radius * np.sin(angle)
            glVertex3f(0.0, y, z)
            glVertex3f(length, y, z)
        glEnd()

    def _draw_joint(self, radius: float = 0.15, slices: int = 16) -> None:
        """Draw sphere at joint location."""
        quad = gluNewQuadric()
        gluSphere(quad, radius, slices, slices)
        gluDeleteQuadric(quad)

    def _draw_arrow(self, length: float, direction: tuple[float, float, float]) -> None:
        """Draw arrow along direction vector."""
        glBegin(GL_LINES)
        glVertex3f(0.0, 0.0, 0.0)
        glVertex3f(
            direction[0] * length,
            direction[1] * length,
            direction[2] * length,
        )
        glEnd()
