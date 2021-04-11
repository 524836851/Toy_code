#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;
const int buffer_len=1024;
int main(int argc,char** argv)
{
    if (argc<3){
        printf("usage: %s server_ip server_port\n",argv[0]);
        exit(0);
    }

    const char* server_ip = argv[1];
    int server_port = atoi(argv[2]);
    printf("%s %s\n",argv[1],argv[2]);

    int client_fd = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET,server_ip,&server_addr.sin_addr);

    int connet_ret = connect(client_fd,(sockaddr*)(&server_addr),sizeof(server_addr));
    if (connet_ret==-1){
        printf("connetc failure\n");
        exit(1);
    }
    char buffer[buffer_len];
    int read_size;
    printf("waitting input:\n");
    while ((read_size =scanf("%s",buffer))!=0){
        if (send(client_fd,buffer,read_size,0)<0){
            printf("send failure\n");
            close(client_fd);
            exit(1);
        }
        int recv_size = recv(client_fd,buffer,buffer_len-1,0);
        if (recv_size <=0) {
            printf("recv failure\n");
            close(client_fd);
            exit(1);
        }
        printf("recv from server: %s\n",buffer);
        printf("waitting input:\n");
    }
    printf("read end!\n");
    close(client_fd);
}