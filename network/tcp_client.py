#-*- coding:utf-8 -*-

from socket import *


client_socket = socket(AF_INET,SOCK_STREAM)
client_socket.connect(('127.0.0.1',2333))
try:
    while True:
        msg = input("input the msg\n")
        client_socket.send(msg.encode())
        #msg = client_socket.recv(2048)
        #print(msg.decode())
except KeyboardInterrupt:
    client_socket.close()
    exit(0)
