#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

static bool stop = false;
static void handle_term(int sig){
    stop = true;
}

int main(int argc,char** argv)
{
    signal(SIGTERM, handle_term);
    if (argc < 4){
        printf("usage: %s ip port backlog\n", argv[0]);
    }
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    // create
    int listen_sock = socket(PF_INET, SOCK_STREAM, 0);

    // create ipv4 addr
    sockaddr_in ipv4_addr;
    bzero(&ipv4_addr, sizeof(ipv4_addr));
    ipv4_addr.sin_family = AF_INET;
    //ipv4_addr.sin_addr.s_addr = inet_addr(argv[1]);
    inet_pton(AF_INET, argv[1], &ipv4_addr.sin_addr);
    ipv4_addr.sin_port = htons(port);

    // bind
    bind(listen_sock, (sockaddr *)(&ipv4_addr), sizeof(ipv4_addr));
    
    // listen
    listen(listen_sock, backlog);
    
    // accept
    while (!stop){
        sleep(1);
    }
    close(listen_sock);
    return 0;
}