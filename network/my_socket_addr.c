#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdio.h>
typedef struct sockaddr_in ipv4_addr;
typedef struct sockaddr_in6 ipv6_addr;
struct sockaddr gneral_addr;


int main()
{
    ipv4_addr addr1;
    gneral_addr = *(struct sockaddr*)(&addr1);
    addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("%d\n", addr1.sin_addr.s_addr);
    printf("%s\n", inet_ntoa(addr1.sin_addr));
    addr1.sin_addr.s_addr = ntohl(addr1.sin_addr.s_addr);
    printf("%d\n", addr1.sin_addr.s_addr);
    printf("%s\n", inet_ntoa(addr1.sin_addr));
}