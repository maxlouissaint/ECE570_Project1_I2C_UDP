'''
use laptop as a remote controller through UDP packet over WiFi link
version 0.1
Date: 10/17/24 
'''
import socket # provides access to low-level networking interface, providing the ability to create and use network connections.

host = '0.0.0.0'  # Listen on all available interfaces. Accept connections from any IP address
port = 8080 # port number the server will listen on for incoming connections

'''
socket.AF_INET: This specifies that the socket will use the IPv4 addressing scheme.
socket.SOCK_STREAM: This indicates that the socket will use the TCP protocol (as opposed to UDP).
'''
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port)) # binds the socket to the specified host and port, meaning it will listen for incoming connections on all network interfaces at port 8080
server_socket.listen(1) # tells the server to listen for incoming connections, with a backlog of 1, meaning it can queue 1 client at a time

print(f"Server listening on {host}:{port}")

while True:
    client_socket, addr = server_socket.accept() # waits for an incoming connection. When a client tries to connect, it accepts the connection and returns a new socket (client_socket) to communicate with the client, and addr, which contains the client's address (IP and port).
    print(f"Connection from {addr}")

    data = client_socket.recv(1024).decode() # reads up to 1024 bytes of data from the client and decode into a string
    print(f"Received: {data}")

    client_socket.sendall("Hello from server\n".encode()) # sends a message back to the client. encode into bytes before sending
    client_socket.close() # close connection after message has been exchanged




