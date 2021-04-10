#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;
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
    sockaddr_in addr_ipv4;
    bzero(&addr_ipv4,0);
    addr_ipv4.sin_family = AF_INET;
    addr_ipv4.sin_port = htons(port);
    //inet_pton(AF_INET,ip,&addr_ipv4.sin_addr);
    addr_ipv4.sin_addr.s_addr = inet_addr(ip);

    int ret_bind = bind(listen_sock,(sockaddr *)(&addr_ipv4),sizeof(addr_ipv4));
    if (ret_bind == -1){
        printf("listen sock bind fail!");
        exit(1);
    }

    listen(listen_sock,5);
    int connect_fd;
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    while ((connect_fd = accept(listen_sock,(sockaddr*)(&client_addr),&client_addr_size))!=-1){
    }
}