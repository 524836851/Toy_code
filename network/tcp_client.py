#-*- coding:utf-8 -*-

from socket import *


client_socket = socket(AF_INET,SOCK_STREAM)
client_socket.connect(('localhost',12000))
try:
    while True:
        msg = input("input the msg\n")
        client_socket.send(msg.encode())
        msg = client_socket.recv(2048)
        print(msg.decode())
except KeyboardInterrupt:
    client_socket.close()
    exit(0)
