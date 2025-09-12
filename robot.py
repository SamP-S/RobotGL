import pygame
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

WINDOW_WIDTH = 480
WINDOW_HEIGHT = 360

cubeVertices = ((-1, -1, -1), (-1, -1, 1), (1, -1, 1), (1, -1, -1), (-1, 1, -1), (-1, 1, 1), (1, 1, 1), (1, 1, -1))
cubeQuads = ((0, 1, 2, 3), (0, 4, 5, 1), (1, 5, 6, 2), (2, 6, 7, 3), (3, 7, 4, 0), (4, 7, 6, 5))
cubeEdges = ((0, 1), (1, 2), (2, 3), (3, 0), (4, 5), (5, 6), (6, 7), (7, 4), (0, 4), (1, 5), (2, 6), (3, 7))
cubePoints = (0, 1, 2, 3, 4, 5, 6, 7)


# definitions
def glWindow():
    pygame.init()
    window = pygame.display
    #pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), DOUBLEBUF|OPENGL)
    pygame.display.set_caption("bruh")
    gluPerspective(45, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, 50)
    glTranslatef(0, 0, -5)
    glScalef(0.5, 0.5, 0.5)


def handleEvents():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()


def quadCube():
    glBegin(GL_QUADS)
    for cubeQuad in cubeQuads:
        for cubeVertex in cubeQuad:
            glVertex3fv(cubeVertices[cubeVertex])
    glEnd()


def edgeCube():
    glBegin(GL_LINES)
    for cubeEdge in cubeEdges:
        for cubeVertex in cubeEdge:
            glVertex3fv(cubeVertices[cubeVertex])
    glEnd()


def pointCube():
    glBegin(GL_POINTS)
    for cubeVertex in cubePoints:
        glVertex3fv(cubeVertices[cubeVertex])
    glEnd()


def main():
    print("pyopengl")
    glWindow()
    frame = 0
    while True:
        handleEvents()
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        glRotatef(0.5, 1, 1.5, 1)
        cube_type = (frame // 60) % 3
        if cube_type == 0:
            quadCube()
        elif cube_type == 1:
            edgeCube()
        else:
            pointCube()
        pygame.display.flip()
        pygame.time.wait(10)
        frame += 1


if __name__ == "__main__":
    main()
