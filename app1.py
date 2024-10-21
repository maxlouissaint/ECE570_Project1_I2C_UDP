'''
uses Pygame for creating a window and handling events, and PyOpenGL for rendering 
a 3D wireframe cube that the user can rotate with the keyboard.
'''
import pygame as pg # creating graphical applications and handling user input
from pygame.locals import *
from OpenGL.GL import * # used for rendering 3D graphics
from OpenGL.GLU import *
import sys # used to exit the program gracefully when a specific key is pressed

Vertices = ((1, 1, 1), (1, 1, -1), (1, -1, -1), (1, -1, 1), (-1, 1, 1), (-1, -1, -1), (-1, -1, 1), (-1, 1, -1)) # 3D coordinates of the cube's 8 corners. Each tuple represents the (x, y, z) coordinates of a vertex.
Edges = ((0, 1), (0, 3), (0, 4), (1, 2), (1, 7), (2, 5), (2, 3), (3, 6), (4, 6), (4, 7), (5, 6), (5, 7)) # define the pairs of vertices that form the 12 edges of the cube.
Quads = ((0, 3, 6, 4), (2, 5, 6, 3), (1, 2, 5, 7), (1, 0, 4, 7), (7, 4, 6, 5), (2, 3, 0, 1)) # define the 6 faces (quads) of the cube. Each face is represented by a tuple of four vertex indices.

def wireCube():
    glBegin(GL_LINES) #starts drawing lines to draw the cube’s edges (wireframe).
    for cubeEdge in Edges: #For each edge in Edges, it draws a line between the two vertices corresponding to that edge
        for vertex in cubeEdge: 
            glVertex3fv(Vertices[vertex])
    glEnd() #Ends the drawing operation for the current shape.



pg.init() # initialize pygame modules
display = (800, 600) # Sets the window size to 800x600 pixels
pg.display.set_mode(display, DOUBLEBUF|OPENGL) # Create a display window using pygame. set to use double buffering with openGL rendering
gluPerspective(45, (display[0]/display[1]), 0.1, 50.0) # Sets up the camera’s perspective. The field of view is 45 degrees, the aspect ratio is 800/600, and objects are visible between distances of 0.1 and 50 units from the camera.
glTranslatef(0.0, 0.0, -5) # Moves the entire scene 5 units away from the camera, so the cube is visible within the perspective.

while True:
    for event in pg.event.get():
        if event.type == pg.KEYDOWN:
            if event.key == pg.K_LEFT: # If the user presses the left arrow key the cube rotates 1 degree around the y-axis
                glRotatef(1, 0, 1, 0)
            if event.key == pg.K_RIGHT: # If the user presses the right arrow key the cube rotates in the opposite direction around the y-axis
                glRotatef(-1, 0, 1, 0)
            if event.key == pg.K_UP: # The up and down arrow keys rotate the cube around the x-axis
                glRotatef(1, 1, 0, 0)
            if event.key == pg.K_DOWN:
                glRotatef(-1, 1, 0, 0)
            if event.key == pg.K_z: # the z and x keys rotate it around the z-axis
                glRotatef(1, 0, 0, 1)
            if event.key == pg.K_x:
                glRotatef(-1, 0, 0, 1)
            if event.key == pg.K_q: # If the 'q' key is pressed, Pygame is quit, and the program exits.
                pg.quit()
                sys.exit()

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) # Clears the color and depth buffers, ensuring a fresh frame is drawn on each iteration of the loop
    wireCube() # draw the wireframe cube
    pg.display.flip() # Updates the display to show the newly drawn frame
    pg.time.wait(10) # wait 10 milliseconds between frames to control the frame rate and make the rotations smoother