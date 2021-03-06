/*
This exzample main for compare LT model and ET model of epoll function
notice:
    The socket used on ET model must set as nonblocking, for it will call the read/write operation repeatly.
————————————————
Level Triggered (LT) 水平触发
.socket接收缓冲区不为空 有数据可读 读事件一直触发
.socket发送缓冲区不满 可以继续写入数据 写事件一直触发
符合思维习惯，epoll_wait返回的事件就是socket的状态

Edge Triggered (ET) 边沿触发
.socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
.socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件
仅在状态变化时触发事件

LT的处理过程：
. accept一个连接，添加到epoll中监听EPOLLIN事件
. 当EPOLLIN事件到达时，read fd中的数据并处理
. 当需要写出数据时，把数据write到fd中；如果数据较大，无法一次性写出，那么在epoll中监听EPOLLOUT事件
. 当EPOLLOUT事件到达时，继续把数据write到fd中；如果数据写出完毕，那么在epoll中关闭EPOLLOUT事件

ET的处理过程：
. accept一个一个连接，添加到epoll中监听EPOLLIN|EPOLLOUT事件
. 当EPOLLIN事件到达时，read fd中的数据并处理，read需要一直读，直到返回EAGAIN为止
. 当需要写出数据时，把数据write到fd中，直到数据全部写完，或者write返回EAGAIN
. 当EPOLLOUT事件到达时，继续把数据write到fd中，直到数据全部写完，或者write返回EAGAIN
————————————————
版权声明：本文为CSDN博主「dongfuye」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dongfuye/article/details/50880251
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>

const int MaxEventNumber = 1024;
const int MaxBuffSize = 10;

void set_nonblocking(int sock_fd){
    int old_option = fcntl(sock_fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(sock_fd,F_SETFL,new_option);
}

void add_fd2epoll(int epollfd,int addfd,bool enable_et)
{
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = addfd;
    if (enable_et) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,addfd,&event);
    set_nonblocking(addfd);
}

/*
    Here input parameters listenfd mainly for recognize listenfd
*/
void et_model(epoll_event* events, int event_num,int epollfd,int listenfd)
{
    for (int i =0;i<event_num;i++){
		int sockfd = events[i].data.fd;
		printf("%d\n",sockfd);
        if (events[i].data.fd == listenfd){
            printf("listen event trigger once\n");
            sockaddr_in client_address;
            socklen_t   client_length = sizeof(client_address);
            int client_fd = accept(listenfd,(sockaddr*)&client_address,&client_length);
            add_fd2epoll(epollfd,client_fd,true);
            continue;
        }
        else if (events[i].events & EPOLLIN){
            printf("EPOLLIN event trigger once!\n");
            char buffer[MaxBuffSize];
            while (true){
                memset(buffer,'\0',MaxBuffSize);
                int ret = recv(events[i].data.fd,buffer,MaxBuffSize-1,0);
                if (ret == 0){
                    //  client sock close socket;
                    printf("client close socket\n");
                    close(events[i].data.fd);
                    break;
                }
                else if (ret < 0 ){
                    if ( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                        printf("Read All this time,wait latter\n");
                    }
                    else{
                        close(events[i].data.fd);
                    }
                    break;
                }
                else {
                   printf("Get message from client: %s\n",buffer);
                }
 
            }
        }
        else{
            printf("Impossible happen\n");
        }
    }
    
}

void lt_model(epoll_event* events,int events_num,int epollfd,int listenfd){
    for (int i =0;i<events_num;i++){
		int sockfd = events[i].data.fd;
		printf("%d\n",sockfd);
        if (events[i].data.fd == listenfd){
            printf("listen event trigger once\n");
            sockaddr_in client_address;
            socklen_t   client_length = sizeof(client_address);
            int client_fd = accept(listenfd,(sockaddr*)&client_address,&client_length);
            add_fd2epoll(epollfd,client_fd,false);
            continue;
        }
        else if (events[i].events & EPOLLIN){
            printf("EPOLLIN event trigger once!\n");
            char buffer[MaxBuffSize];
            memset(buffer,'\0',MaxBuffSize);
            int ret = recv(events[i].data.fd,buffer,MaxBuffSize-1,0);
            if (ret <= 0){
                //  client sock close socket;
                printf("client close socket\n");
                close(events[i].data.fd);
                break;
            }
            else {
               printf("Get message from client: %s\n",buffer);
            }
        }
        else{
            printf("Impossible happen\n");
        }
    }
}

void lt_model_2(epoll_event* events,int events_num,int epollfd,int listenfd){
    for (int i =0;i<events_num;i++){
		int sockfd = events[i].data.fd;
		printf("%d\n",sockfd);
        if (events[i].data.fd == listenfd){
            printf("listen event trigger once\n");
            sockaddr_in client_address;
            socklen_t   client_length = sizeof(client_address);
            int client_fd = accept(listenfd,(sockaddr*)&client_address,&client_length);
            add_fd2epoll(epollfd,client_fd,false);
            continue;
        }
        else if (events[i].events & EPOLLIN){
            printf("EPOLLIN event trigger once!\n");
            char buffer[MaxBuffSize];
            while (true){
                memset(buffer,'\0',MaxBuffSize);
                int ret = recv(events[i].data.fd,buffer,MaxBuffSize-1,0);
                if (ret == 0){
                    //  client sock close socket;
                    printf("client close socket\n");
                    close(events[i].data.fd);
                    break;
                }
                else if (ret < 0 ){
                    if ( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                        printf("Read All this time,wait latter\n");
                    }
                    else{
                        close(events[i].data.fd);
                    }
                    break;
                }
                else {
                   printf("Get message from client: %s\n",buffer);
                }
            }
        }
        else{
            printf("Impossible happen\n");
        }
    }
}
int main(int argc,char* argv[])
{
    if (argc < 4){
        printf("usage is: %s ip_address port_number use_et\n",argv[0]);
        exit(1);
    }
    int port_number = atoi(argv[2]);
    int use_et = atoi(argv[3]);
    int ret;

    sockaddr_in server_ip;
    bzero(&server_ip,sizeof(server_ip));
    server_ip.sin_family = AF_INET;
    inet_pton(AF_INET,argv[1],&server_ip.sin_addr);
    server_ip.sin_port = htons(port_number);

    int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	printf("listen sock is %d\n",listen_sock);

    ret  = bind(listen_sock,(sockaddr*)(&server_ip),sizeof(server_ip));

    ret = listen(listen_sock,5);
    
    epoll_event events[MaxEventNumber];
    int epollfd = epoll_create(5);
    add_fd2epoll(epollfd,listen_sock,true);
    
    while (true){
        printf("Waiting epoll events\n");
        ret = epoll_wait(epollfd,events,MaxEventNumber,-1);

        if (ret < 0){
            printf("epoll wait error\n");
            break;
        }
        if (use_et){
            et_model(events,ret,epollfd,listen_sock);
        }
        else{
            lt_model_2(events,ret,epollfd,listen_sock);
        }
    }


    return 0;
}
