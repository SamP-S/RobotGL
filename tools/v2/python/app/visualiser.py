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
    
    def _pushCameraTransform(self):
        glPushMatrix()
        glTranslatef(0, 0, -5)
        glRotatef(30, 1, 0, 0)
        speed = 360 / 10000  # degrees per ms
        elapsed_time = pg.time.get_ticks() - self.start_time
        glRotatef(speed * elapsed_time, 0, 1, 0)
        glScalef(500, 500, 500)
        
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
                
    def drawRobot(self, robot):
        pass  # Placeholder for robot drawing logic
    
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
        
    # def drawPose(self, pose, colour=(0,1,0)):
    #     glBegin(GL_LINES)
    #     for i in range(len(pose) - 1):
    #         glColor3fv(colour)
    #         glVertex3fv(pose[i])
    #         glVertex3fv(pose[i + 1])
    #     glEnd()

    def drawPoint(self, point, colour=(1,0,0)):
        glColor3fv(colour)
        glBegin(GL_POINTS)
        glVertex3fv(point)
        glEnd()
    
    def end_frame(self):
        pg.display.flip()
        self.frame += 1
        self._popCameraTransform()
