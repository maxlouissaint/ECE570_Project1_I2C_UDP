'''
uses Pygame for creating a window and handling events, and PyOpenGL for rendering 
a 3D wireframe cube that the user can rotate with the keyboard.
'''
import pygame as pg # creating graphical applications and handling user input
from pygame.locals import *
from OpenGL.GL import * # used for rendering 3D graphics
from OpenGL.GLU import *
import sys # used to exit the program gracefully when a specific key is pressed

from socket import *
import numpy as np
from time import time, strftime
from numpy import frombuffer, array, savetxt
from tkinter import Tk, Label, Button, scrolledtext
from tkinter import INSERT, END
from threading import Thread
from functools import partial
from datetime import datetime
import struct

# Set up UDP server
UDP_IP = "0.0.0.0"  # Assigned IP address to socket. 0.0.0.0 is used as a way for the server to listen to all the interfaces of this PC.
UDP_PORT = 6000 # Assigned port to socket. Port will be different
sock = socket(AF_INET, SOCK_DGRAM) ## Create a socket object with IPv4 and UDP protocol
sock.bind((UDP_IP, UDP_PORT)) #bind socket to the port and IP defined above
print('server bind at ', sock) #print successful binding message
BUFFER_SIZE = 8 # sets the size of the buffer that will be used to hold incoming data packets


Vertices = ((1, 1, 1), (1, 1, -1), (1, -1, -1), (1, -1, 1), (-1, 1, 1), (-1, -1, -1), (-1, -1, 1), (-1, 1, -1)) # 3D coordinates of the cube's 8 corners. Each tuple represents the (x, y, z) coordinates of a vertex.
Edges = ((0, 1), (0, 3), (0, 4), (1, 2), (1, 7), (2, 5), (2, 3), (3, 6), (4, 6), (4, 7), (5, 6), (5, 7)) # define the pairs of vertices that form the 12 edges of the cube.
Quads = ((0, 3, 6, 4), (2, 5, 6, 3), (1, 2, 5, 7), (1, 0, 4, 7), (7, 4, 6, 5), (2, 3, 0, 1)) # define the 6 faces (quads) of the cube. Each face is represented by a tuple of four vertex indices.

def wireCube():
    glBegin(GL_LINES) #starts drawing lines to draw the cube’s edges (wireframe).
    for cubeEdge in Edges: #For each edge in Edges, it draws a line between the two vertices corresponding to that edge
        for vertex in cubeEdge: 
            glVertex3fv(Vertices[vertex])
    glEnd() #Ends the drawing operation for the current shape.

def unpack_bytes_to_float(buffer, start_index):
    # Extract 4 bytes from the buffer
    byte_data = buffer[start_index:start_index + 4]  # Get the relevant 4 bytes
    # Unpack the bytes into a float
    value = struct.unpack('>f', byte_data)  # '>f' means big-endian float
    return value[0]  # Return the first element of the tuple


# Initialize Pygame and OpenGL
pg.init() # initialize pygame modules
display = (800, 600) # Sets the window size to 800x600 pixels
pg.display.set_mode(display, DOUBLEBUF|OPENGL) # Create a display window using pygame. set to use double buffering with openGL rendering
gluPerspective(45, (display[0]/display[1]), 0.1, 50.0) # Sets up the camera’s perspective. The field of view is 45 degrees, the aspect ratio is 800/600, and objects are visible between distances of 0.1 and 50 units from the camera.
glTranslatef(0.0, 0.0, -5) # Moves the entire scene 5 units away from the camera, so the cube is visible within the perspective.


# Create a custom event that triggers every (10 ms)
AUTO_ROTATE_EVENT = pg.USEREVENT + 1
pg.time.set_timer(AUTO_ROTATE_EVENT, 10)  # Set the timer for 1-second intervals

while True:
    data, addr = sock.recvfrom(BUFFER_SIZE)
    #if len(data) == 8:  # Ensure we've received 8 bytes (two floats for roll and pitch)
    roll, pitch = struct.unpack('>ff', data)  # Unpack two floats. Converts the 8-byte data into two floats corresponding to roll and pitch.
    # Unpack the same data as two unsigned 32-bit integers for binary representation
    # roll_b = struct.unpack('I', data[:4])[0]  # First 4 bytes for roll
    # pitch_b = struct.unpack('I', data[4:])[0]  # Next 4 bytes for pitch
    print(f'Received roll: {roll}, pitch: {pitch}, from: {addr}\n')
    # print(f'Binary roll: {format(roll_b, "032b")}\n')
    # print(f'Binary pitch: {format(pitch_b, "032b")}\n')
    
    # Initial rotation
    glRotatef(1, 1, 0, 0)
    
    for event in pg.event.get():
       if event.type == AUTO_ROTATE_EVENT:
           if np.abs(pitch) > np.abs(roll):
               # Apply pitch rotation around the X-axis
               if (pitch > 0):
                   glRotatef(1, 0, 1, 0)
               elif (pitch < 0):
                   glRotatef(-1, 0, 1, 0)
               # Apply roll rotation around the z-axis  
           else:
               if (roll > 0):
                   glRotatef(1, 0, 0, 1)
               elif(roll < 0):
                   glRotatef(-1, 0, 0, 1)
       if event.type == pg.KEYDOWN:
           if event.key == pg.K_q: # If the 'q' key is pressed, Pygame is quit, and the program exits.
               pg.quit()
               sys.exit()
               
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) # Clears the color and depth buffers, ensuring a fresh frame is drawn on each iteration of the loop and apply the orientation transformations
  
    
    wireCube() # draw the wireframe cube
    pg.display.flip() # Updates the display to show the newly drawn frame
    pg.time.wait(100) # wait 100 milliseconds between frames to control the frame rate and make the rotations smoother