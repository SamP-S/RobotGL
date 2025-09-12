import pygame as pg
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

from math import *

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

def testRobot():
    q1 = 1 + 1 * sin(pg.time.get_ticks() / 1000)
    q2 = -45 + 90 * sin(pg.time.get_ticks() / 1000)
    
    drawOrigin()
    applyDH(2, 0, 0, q2)
    drawOrigin()
    applyDH(2, 90, 2, q2)
    drawOrigin()

def scaraRobot(q):
    q0, q1, q2, q3 = q
    
    # static link
    L0_LINK_OFFSET = 2.0    # d
    L0_JOINT_ANGLE = 0.0    # theta
    L0_LINK_LENGTH = 0.0    # a
    L0_LINK_TWIST = 0.0    # alpha
    # rotational
    L1_LINK_OFFSET = 0.0    # d
    L1_JOINT_ANGLE = q[0]    # theta
    L1_LINK_LENGTH = 1.0    # a
    L1_LINK_TWIST = 0.0    # alpha
    # rotational
    L2_LINK_OFFSET = 0.0    # d
    L2_JOINT_ANGLE = q[1]    # theta
    L2_LINK_LENGTH = 1.0    # a
    L2_LINK_TWIST = 0.0    # alpha
    # rotational
    L3_LINK_OFFSET = -0.5    # d
    L3_JOINT_ANGLE = q[2]    # theta
    L3_LINK_LENGTH = 0.0    # a
    L3_LINK_TWIST = 180.0    # alpha
    # prismatic
    L4_LINK_OFFSET = q[3]    # d
    L4_JOINT_ANGLE = 0.0    # theta
    L4_LINK_LENGTH = 0.0    # a
    L4_LINK_TWIST = 0.0    # alpha
    
    drawOrigin()
    applyDH(
        L0_LINK_OFFSET,
        L0_JOINT_ANGLE,
        L0_LINK_LENGTH,
        L0_LINK_TWIST
    )
    drawOrigin()
    applyDH(
        L1_LINK_OFFSET,
        L1_JOINT_ANGLE,
        L1_LINK_LENGTH,
        L1_LINK_TWIST
    )
    drawOrigin()
    applyDH(
        L2_LINK_OFFSET,
        L2_JOINT_ANGLE,
        L2_LINK_LENGTH,
        L2_LINK_TWIST
    )
    drawOrigin()
    applyDH(
        L3_LINK_OFFSET,
        L3_JOINT_ANGLE,
        L3_LINK_LENGTH,
        L3_LINK_TWIST
    )
    drawOrigin()
    applyDH(
        L4_LINK_OFFSET,
        L4_JOINT_ANGLE,
        L4_LINK_LENGTH,
        L4_LINK_TWIST
    )
    drawOrigin()
    
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

        # testRobot()
        q = [
            -30.0,
            -30.0,
            -30.0,
            q3
        ]
        scaraRobot(q)
        # drawOrigin()
        
        glPopMatrix()
        pg.display.flip()
        pg.time.wait(10)
        
        frame += 1


if __name__ == "__main__":
    main()
