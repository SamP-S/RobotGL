import pygame as pg
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

from math import *
import numpy as np
import roboticstoolbox as rtb

WINDOW_WIDTH = 480
WINDOW_HEIGHT = 360

# definitions
def glWindow():
    pg.init()
    window = pg.display
    pg.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), DOUBLEBUF|OPENGL)
    pg.display.set_caption("bruh")
    # camera to world transformations
    gluPerspective(45, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, 50)
    glTranslatef(0, -1, -5)
    glRotatef(30, 1, 0, 0)
    glRotatef(45, 0, 1, 0)
    glScalef(0.5, 0.5, 0.5)
    
    # opengl settings
    glLineWidth(5.0)
    

def handleEvents():
    for event in pg.event.get():
        if event.type == pg.QUIT:
            pg.quit()
            quit()
            
# d: link offset    (prismatic)
# theta: joint angle    (theta)
# a: link length       
# alpha: link twist   
def applyDH(d, theta, a, alpha):
    # draw variable transforms
    glColor3fv((1, 1, 0))
    glBegin(GL_LINES)
    glVertex3fv((0, 0, 0))
    glVertex3fv((0, 0, d))
    glEnd()
    # apply variable transforms
    glRotatef(theta, 0, 0, 1)
    glTranslate(0, 0, d)
    
    # draw static transforms
    glColor3fv((1, 0, 1))
    glBegin(GL_LINES)
    glVertex3fv((0, 0, 0))
    glVertex3fv((a, 0, 0))
    glEnd()
    # apply static transforms
    glTranslate(a, 0, 0)
    glRotatef(alpha, 1, 0, 0)
    
def drawOrigin():
    axes = [(0, 0, 1), (0, 1, 0), (1, 0, 0)]
    
    glBegin(GL_LINES)
    for axis in axes:
        glColor3fv(axis)
        glVertex3fv((0, 0, 0))
        glVertex3fv(axis)
    glEnd()
    
### MATHEMATICS ###

def dhMatrix(d, theta, a, alpha):
    theta = np.radians(theta)
    alpha = np.radians(alpha)
    return np.array([
        [np.cos(theta), -np.sin(theta)*np.cos(alpha), np.sin(theta)*np.sin(alpha), a*np.cos(theta)],
        [np.sin(theta), np.cos(theta)*np.cos(alpha), -np.cos(theta)*np.sin(alpha), a*np.sin(theta)],
        [0, np.sin(alpha), np.cos(alpha), d],
        [0, 0, 0, 1]
    ], dtype=float)
    
    
### ROBOTS ###
    
def sampleRobot():
    robot = rtb.DHRobot(
        links=[
            rtb.robot.DHLink.RevoluteDH(d=2, theta=30, a=0, alpha=0),
            rtb.robot.DHLink.RevoluteDH(d=0, theta=30, a=2, alpha=0)
        ],
        name="Sample 2R Robot"
    )
    return robot

def scaraRobot():
    return rtb.models.DH.Cobra600()

### OPERATIONS ###

def testRobot(robot):
    drawOrigin()
    for link in robot:
        d, theta, a, alpha = link
        applyDH(d, theta, a, alpha)
        drawOrigin()

def main():
    print("pyrobot")
    glWindow()
    frame = 0
    
    scara = scaraRobot()
    sample = sampleRobot()

    print(f"SCARA:\n{scara}")
    print(f"SAMPLE:\n{sample}")

    while True:
        handleEvents()
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glPushMatrix()
        
        glRotatef(-90, 1, 0, 0)

        q = [
            -45 + 45 * cos(pg.time.get_ticks() / 1000),
            -45 + 45 * cos(pg.time.get_ticks() / 1000),
            -45 + 45 * cos(pg.time.get_ticks() / 1000),
            1.5
        ]
        
        # tests
        Te = r.fkine(r.qr)
        # testRobot(r)
        # forward(r)
        
        glPopMatrix()
        pg.display.flip()
        pg.time.wait(10)
        
        frame += 1


if __name__ == "__main__":
    main()
