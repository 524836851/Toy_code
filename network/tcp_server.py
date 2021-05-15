#-*- coding:utf-8 -*-

from socket import *
import time

server_socket = socket(AF_INET,SOCK_STREAM)
server_socket.bind(('localhost',12000))
server_socket.listen(2)
print("Waiting Tcp connect")
connect_socket,client_addr = server_socket.accept()
print("accept ",client_addr)
while True:
    time.sleep(1) 
    connect_socket.send("hello world!".encode());
    