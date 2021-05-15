#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc,char* argv[]){
    if (argc < 2){
        printf("Usage: %s hostname\n",argv[0]);
        exit(1);
    }
    struct hostent* result;
    result = gethostbyname(argv[1]);
    char hostaddr[100];
    inet_ntop(result->h_addrtype,result->h_addr_list[0],hostaddr,sizeof(hostaddr));
    printf("IP is %s\n",hostaddr);
    return 0;
}
