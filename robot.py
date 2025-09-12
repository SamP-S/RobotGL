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
    gluPerspective(45, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, 50)
    glTranslatef(0, -1, -5)
    glScalef(0.5, 0.5, 0.5)
    glRotatef(-90, 1, 0, 0)
    glRotatef(-45, 0, 0, 1)
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
    
    
def main():
    print("pyrobot")
    glWindow()
    frame = 0
    while True:
        handleEvents()
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glPushMatrix()
        
        # glRotatef(0.5 * frame, 1, 1.5, 1)
        q1 = 1 + 1 * sin(pg.time.get_ticks() / 1000)
        q2 = -45 + 90 * sin(pg.time.get_ticks() / 1000)
        
        drawOrigin()
        
        applyDH(2, 0, 0, q2)
        
        drawOrigin()
        applyDH(2, 90, 2, q2)
        drawOrigin()
        
        # applyDH(2, 0, 0, 0)
        # drawOrigin()
        
        glPopMatrix()
        pg.display.flip()
        pg.time.wait(10)
        
        frame += 1


if __name__ == "__main__":
    main()
