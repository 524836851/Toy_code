#include <iostream>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;
const int buffer_len = 1024;
int main(int argc,char** argv)
{
    if (argc<3){
        printf("usage: %s ip port\n",argv[0]);
        exit(1);
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // create socket
    int listen_sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr_ipv4;
    bzero(&addr_ipv4,sizeof(addr_ipv4));
    addr_ipv4.sin_family = AF_INET;
    addr_ipv4.sin_port = htons(port);
    inet_pton(AF_INET,ip,&addr_ipv4.sin_addr);
    //addr_ipv4.sin_addr.s_addr = inet_addr(ip);

    int ret_bind = bind(listen_sock,(sockaddr *)(&addr_ipv4),sizeof(addr_ipv4));
    if (ret_bind == -1){
        close(listen_sock);
        printf("listen sock bind fail!\n");
        exit(1);
    }

    if (listen(listen_sock,5) == -1){
        close(listen_sock);
        printf("listen sock listen fail!\n");
        exit(1);
    }

    int connect_fd;
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    printf("watiting accept next connet...\n");
    while ((connect_fd = accept(listen_sock,(sockaddr*)(&client_addr),&client_addr_size))!=-1){
        char temp[INET_ADDRSTRLEN];
        printf("connetct with ip:%s port:%d\n",inet_ntop(client_addr.sin_family,&client_addr.sin_addr,temp,INET6_ADDRSTRLEN),ntohs(client_addr.sin_port));
        char buffer[buffer_len];
		memset(buffer,'\0',buffer_len);
        int recv_ret;
        while ((recv_ret = recv(connect_fd,buffer,buffer_len-1,0))>0){
            printf("recv message:%s\n",buffer);
            for (int i =0;i<recv_ret;i++){
                if (buffer[i]>='a' && buffer[i] <= 'z'){
                    buffer[i] += 'A'-'a';
                }
            }
            int send_ret = send(connect_fd,buffer,recv_ret,0);
            if (send_ret <0 ){
                printf("send failure\n");
                break;
            }
			memset(buffer,'\0',buffer_len);
        }
        close(connect_fd);
        printf("watiting accept next connet...\n");
    }
    close(listen_sock);
    printf("acppet failure!\n");
    exit(1);
}
