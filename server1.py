'''
use laptop as a remote controller through UDP packet over WiFi link
version 0.1
Date: 10/17/24 
'''
import socket

host = '0.0.0.0'  # Listen on all available interfaces
port = 8080

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((host, port))
server_socket.listen(1)

print(f"Server listening on {host}:{port}")

while True:
    client_socket, addr = server_socket.accept()
    print(f"Connection from {addr}")

    data = client_socket.recv(1024).decode()
    print(f"Received: {data}")

    client_socket.sendall("Hello from server\n".encode())
    client_socket.close()




