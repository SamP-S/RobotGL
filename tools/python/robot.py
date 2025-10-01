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
    
def sampleRobot(q):
    q1, q2 = q
    l0 = [2, q1, 0, 0]
    l1 = [2, q2, 0, 0]
    return [l0, l1]

def scaraRobot(q):
    q0, q1, q2, q3 = q
    # [d, theta, a, alpha]
    # [link offset, joint angle, link length, link twist]
    l0 = [2.0, q[0], 1.0, 0.0]
    l1 = [0.0, q[1], 1.0, 0.0]
    l2 = [-0.5, q[2], 0.0, 180.0]
    l3 = [q[3], 0.0, 0.0, 0.0]
    return [l0, l1, l2, l3]

def testRobot(robot):
    drawOrigin()
    for link in robot:
        d, theta, a, alpha = link
        applyDH(d, theta, a, alpha)
        drawOrigin()

def forward(q):
    pass

def inverse(pos):
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
        qx = -30 +15 * cos(pg.time.get_ticks() / 1000)

        # testRobot()
        q = [
            qx,
            qx,
            qx,
            q3
        ]
        
        r = scaraRobot(q)
        testRobot(r)
        
        glPopMatrix()
        pg.display.flip()
        pg.time.wait(10)
        
        frame += 1


if __name__ == "__main__":
    main()
