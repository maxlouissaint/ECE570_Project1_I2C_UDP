import pygame as pg
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import sys

Vertices = ((1, 1, 1), (1, 1, -1), (1, -1, -1), (1, -1, 1), (-1, 1, 1), (-1, -1, -1), (-1, -1, 1), (-1, 1, -1))
Edges = ((0, 1), (0, 3), (0, 4), (1, 2), (1, 7), (2, 5), (2, 3), (3, 6), (4, 6), (4, 7), (5, 6), (5, 7))
Quads = ((0, 3, 6, 4), (2, 5, 6, 3), (1, 2, 5, 7), (1, 0, 4, 7), (7, 4, 6, 5), (2, 3, 0, 1))

def wireCube():
    glBegin(GL_LINES)
    for cubeEdge in Edges:
        for vertex in cubeEdge:
            glVertex3fv(Vertices[vertex])
    glEnd()


pg.init()
display = (800, 600)
pg.display.set_mode(display, DOUBLEBUF|OPENGL)
gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)
glTranslatef(0.0, 0.0, -5)

while True:
    for event in pg.event.get():
        if event.type == pg.KEYDOWN:
            if event.key == pg.K_LEFT:
                glRotatef(1, 0, 1, 0)
            if event.key == pg.K_RIGHT:
                glRotatef(-1, 0, 1, 0)
            if event.key == pg.K_UP:
                glRotatef(1, 1, 0, 0)
            if event.key == pg.K_DOWN:
                glRotatef(-1, 1, 0, 0)
            if event.key == pg.K_z:
                glRotatef(1, 0, 0, 1)
            if event.key == pg.K_x:
                glRotatef(-1, 0, 0, 1)
            if event.key == pg.K_q:
                pg.quit()
                sys.exit()

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    wireCube()
    pg.display.flip()
    pg.time.wait(10)

