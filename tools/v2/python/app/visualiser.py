import pygame as pg
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

from math import *
import numpy as np

WINDOW_WIDTH = 480
WINDOW_HEIGHT = 360

class Visualiser:
    
    def __init__(self):
        self.has_window = False
        self.frame = 0
        self.start_time = pg.time.get_ticks()
    
    def createWindow(self):
        if (self.has_window):
            return False
        
        pg.init()
        window = pg.display
        pg.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), DOUBLEBUF|OPENGL)
        pg.display.set_caption("Robot Visualiser")
        self.has_window = True
        
        # camera to world transformations
        gluPerspective(45, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, 50)
        
        # opengl settings
        glLineWidth(2.0)
        glPointSize(3.0)
        return True
    
    def getTicks(self):
        return pg.time.get_ticks()
    
    def _pushCameraTransform(self):
        glPushMatrix()
        glTranslatef(0, 0, -5)
        glRotatef(30, 1, 0, 0)
        speed = 360 / 40000  # degrees per ms
        elapsed_time = pg.time.get_ticks() - self.start_time
        glRotatef(speed * elapsed_time, 0, 1, 0)
        glScalef(5, 5, 5)
        
    def _popCameraTransform(self):
        glPopMatrix()
    
    def begin_frame(self):
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
                quit()
        self._pushCameraTransform()

    def clear(self):
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    
    def _drawLink(self, d, theta, a, alpha):
        """Draw visual helpers for a DH link in the current frame.

        Draw the a length (X axis) in yellow and d length (Z axis) in magenta.
        """
        # Convert theta to degrees for GL
        theta_deg = float(theta) * 180.0 / pi

        # We must apply Rz(theta) before drawing the a/d lines because
        # DH order is Rz(theta) Tz(d) Tx(a) Rx(alpha).
        # Draw 'd' along local Z after Rz(theta)
        if d is not None and abs(d) > 1e-9:
            glPushMatrix()
            glRotatef(theta_deg, 0.0, 0.0, 1.0)
            glColor3f(1.0, 0.0, 1.0)
            glBegin(GL_LINES)
            glVertex3f(0.0, 0.0, 0.0)
            glVertex3f(0.0, 0.0, d)
            glEnd()
            glPopMatrix()

        # Draw 'a' along local X after Rz(theta) and translation along Z by d
        if a is not None and abs(a) > 1e-9:
            glPushMatrix()
            glRotatef(theta_deg, 0.0, 0.0, 1.0)
            # move to the point after Tz(d)
            glTranslatef(0.0, 0.0, float(d))
            glColor3f(1.0, 1.0, 0.0)
            glBegin(GL_LINES)
            glVertex3f(0.0, 0.0, 0.0)
            glVertex3f(a, 0.0, 0.0)
            glEnd()
            glPopMatrix()


    def _transformDH(self, d, theta, a, alpha):
        """Apply a DH transform: Rz(theta) Tz(d) Tx(a) Rx(alpha).

        Angles are expected in radians.
        """
        theta_deg = float(theta) * 180.0 / pi
        alpha_deg = float(alpha) * 180.0 / pi

        # Rz(theta)
        glRotatef(theta_deg, 0.0, 0.0, 1.0)
        # Tz(d)
        glTranslatef(0.0, 0.0, float(d))
        # Tx(a)
        glTranslatef(float(a), 0.0, 0.0)
        # Rx(alpha)
        glRotatef(alpha_deg, 1.0, 0.0, 0.0)
        

    def drawRobot(self, robot, q=None, size=0.01):
        """Draw a Robotics Toolbox robot (rtb.DHRobot)

        robot: rtb.DHRobot instance
        q: optional joint list (length robot.n). If None, zeros are used.
        size: axis scale when drawing joint origins.
        """
        if q is None:
            q = [0.0] * robot.n

        # Start from base frame
        glPushMatrix()
        # draw base origin
        self.drawOrigin(size)

        # iterate through joints
        pushes = 1
        for i in range(robot.n):
            
            link = robot.links[i]
            a = getattr(link, 'a', 0.0)
            d = getattr(link, 'd', 0.0)
            alpha = getattr(link, 'alpha', 0.0)
            offset = getattr(link, 'offset', 0.0)
            theta = q[i] + offset

            # push matrix for this joint so we can pop after
            glPushMatrix()
            pushes += 1

            # draw the link lengths in this local frame
            self._drawLink(d, theta, a, alpha)

            # apply DH transform to move to next joint
            self._transformDH(d, theta, a, alpha)

            # draw the joint origin at the new frame
            self.drawOrigin(size)

        # pop all matrices (one per joint plus the initial base)
        for _ in range(pushes):
            glPopMatrix()
    
    def drawOrigin(self, size=1):
        glBegin(GL_LINES)
        # X axis in red
        glColor3fv((1, 0, 0))
        glVertex3fv((0, 0, 0))
        glVertex3fv((size, 0, 0))
        # Y axis in green
        glColor3fv((0, 1, 0))
        glVertex3fv((0, 0, 0))
        glVertex3fv((0, size, 0))
        # Z axis in blue
        glColor3fv((0, 0, 1))
        glVertex3fv((0, 0, 0))
        glVertex3fv((0, 0, size))
        glEnd()

    def drawPoint(self, point, colour=(1,0,0)):
        glColor3fv(colour)
        glBegin(GL_POINTS)
        glVertex3fv(point)
        glEnd()
    
    def end_frame(self):
        pg.display.flip()
        self.frame += 1
        self._popCameraTransform()
