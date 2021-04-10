#-*- coding:utf-8 -*-

from socket import *


server_socket = socket(AF_INET,SOCK_DGRAM)
server_socket.bind(('',12000))

while True:
    print("Waiting client info")
    msg,client_info= server_socket.recvfrom(2048)
    msg = msg.decode().upper()
    print("Get Msg! send to",client_info)
    server_socket.sendto(msg.encode(),client_info)