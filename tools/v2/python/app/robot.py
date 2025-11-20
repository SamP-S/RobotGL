import pygame as pg
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

from math import *
import numpy as np

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
    
### ROBOTS ###
    
def sampleRobot(q):
    q1, q2 = q
    l0 = np.array([2, q1, 0, 0], dtype=float)
    l1 = np.array([2, q2, 0, 0], dtype=float)
    return [l0, l1]

def scaraRobot(q):
    q0, q1, q2, q3 = q
    # [d, theta, a, alpha]
    # [link offset, joint angle, link length, link twist]
    l0 = np.array([2.0, q0, 1.0, 0.0], dtype=float)
    l1 = np.array([0.0, q1, 1.0, 0.0], dtype=float)
    l2 = np.array([-0.5, q2, 0.0, 180.0], dtype=float)
    l3 = np.array([q3, 0.0, 0.0, 0.0], dtype=float)
    return [l0, l1, l2, l3]

def testRobot(robot):
    drawOrigin()
    for link in robot:
        d, theta, a, alpha = link
        applyDH(d, theta, a, alpha)
        drawOrigin()

def dhMatrix(d, theta, a, alpha):
    theta = np.radians(theta)
    alpha = np.radians(alpha)
    return np.array([
        [np.cos(theta), -np.sin(theta)*np.cos(alpha), np.sin(theta)*np.sin(alpha), a*np.cos(theta)],
        [np.sin(theta), np.cos(theta)*np.cos(alpha), -np.cos(theta)*np.sin(alpha), a*np.sin(theta)],
        [0, np.sin(alpha), np.cos(alpha), d],
        [0, 0, 0, 1]
    ], dtype=float)

# forward kinematics
def forward(r):
    # initial position
    pos = np.zeros((4, 1))
    pos[3, 0] = 1
    # iteratively compute transformation matrix
    T = np.eye(4)
    for link in r:
        d, theta, a, alpha = link
        A = dhMatrix(d, theta, a, alpha)
        T = np.matmul(T, A)
    end = np.matmul(T, pos)
    # Format output to .2f
    print(f"End effector position: {[[f'{v[0]:.2f}' for v in end]]}")
    return end

# inverse kinematics (newton-raphson)
def inverse(r0, q0, target, max_iter=1000, tol=1e-3):
    # Xn1 = Xn0 - f(Xn0) / f'(Xn0)
    for i in range(max_iter):
        pass
    
    
def main():
    print("pyrobot")
    glWindow()
    frame = 0
    while True:
        handleEvents()
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glPushMatrix()
        
        glRotatef(-90, 1, 0, 0)
        q3 = 1 + 0.5 * sin(pg.time.get_ticks() / 1000)
        # qx = -30 +15 * cos(pg.time.get_ticks() / 1000)
        qx = -45 + 45 * cos(pg.time.get_ticks() / 1000)

        # testRobot()
        q = [
            qx,
            qx,
            qx,
            1.5
        ]
        
        r = scaraRobot(q)
        
        # tests
        testRobot(r)
        forward(r)
        
        glPopMatrix()
        pg.display.flip()
        pg.time.wait(10)
        
        frame += 1


if __name__ == "__main__":
    main()
