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
            
def dhMatrix(d, theta, a, alpha):
    theta = radians(theta)
    alpha = radians(alpha)
    return [
        [cos(theta), -sin(theta)*cos(alpha), sin(theta)*sin(alpha), a*cos(theta)],
        [sin(theta), cos(theta)*cos(alpha), -cos(theta)*sin(alpha), a*sin(theta)],
        [0, sin(alpha), cos(alpha), d],
        [0, 0, 0, 1]
    ]
    
def identityMatrix():
    return [
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ]

def mulMatrix(m1, m2):
    # assume row major matrices
    m1_m, m1_n = len(m1), len(m1[0])
    m2_m, m2_n = len(m2), len(m2[0])
    if m1_n != m2_m:
        raise ValueError("Cannot multiply matrices: dimensions do not match")

    # perform multiplication
    result = [[0 for _ in range(m2_n)] for _ in range(m1_m)]
    for i in range(m1_m):
        for j in range(m2_n):
            for k in range(m1_n):
                result[i][j] += m1[i][k] * m2[k][j]
    return result

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

# forward kinematics
def forward(r):
    pos = [[0], [0], [0], [1]]
    T = identityMatrix()
    for link in r:
        d, theta, a, alpha = link
        A = dhMatrix(d, theta, a, alpha)
        T = mulMatrix(T, A)
    end = mulMatrix(T, pos)
    # Format output to .2f
    print(f"End effector position: {[[f'{v[0]:.2f}' for v in end]]}")
    
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
