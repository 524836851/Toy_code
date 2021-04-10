#-*- coding:utf-8 -*-

from socket import * 

dest_info = ('localhost',12000)

client_socket = socket(AF_INET,SOCK_DGRAM)
client_socket.bind(('localhost',28802))

try :
    while True:
        read_str = input()
        client_socket.sendto(read_str.encode(),dest_info)
        msg,server_addres = client_socket.recvfrom(2048)
        print(msg.decode())
except KeyboardInterrupt:
    client_socket.close()
    exit(0)
