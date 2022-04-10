#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>

#define NUM 100

int serverSocket;

struct epoll_event events[NUM];

void exit_handler()
{
    for (int i = 0; i < NUM; i++)
    {
        if (-1 == events[i].data.fd)
        {
            close(events[i].data.fd);
            events[i].data.fd = -1;
        }
    }
    printf("bye!\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        printf("don't have enough arg!\n");
        exit(0);
    }
    // signal exit()
    signal(2, exit_handler);
    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serverSocket) printf("don't get serverSocket_Id!%m\n"),exit(-1);
    printf("Get the serverSocket!\n");

    // create server protrol address family
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(argv[1]);
    add.sin_port = atoi(argv[2]);

    // bind
    int r = bind(serverSocket, (struct sockaddr_in *)&add,sizeof(add));
    if(-1==r) printf("bind failed!%m\n"),exit(-1);
    printf("bind successful!\n");

    // listen
    r = listen(serverSocket, 10);
    if(-1==r) printf("listen failed!%m\n"),exit(-1);
    printf("listen successful!\n");

    // watch
    struct sockaddr_in add_cliect = {0};
    int len_cliect = sizeof(add_cliect);
    char buffer[256];

        // create epoll
    int epfd = epoll_create(5000);
    if(epfd<0){
        printf("create epoll failed! %m\n");
        close(serverSocket);
        exit(-1);
    }
    printf("create epoll successfully !\n");
        // registe the event
    struct epoll_event ev; //  use when create epoll event 
    
    for(int i =0;i<NUM;i++){
        events[i].data.fd = -1;
    }
    
    // first put the socket of server into the array;
    ev.data.fd = serverSocket;
    ev.events = EPOLLIN;

    r = epoll_ctl(epfd, EPOLL_CTL_ADD, serverSocket, &ev);
    if(r<0){
        printf("add serverSocket failed! %m\n");
        close(epfd);
        close(serverSocket);
        exit(-2);
    }
    printf("add serverSocket successfully\n");
    int num_of_fds=0; // the return value of epoll_wait
    while (1)
    {   
        num_of_fds = epoll_wait(epfd, events, 5000, 0);
        if(num_of_fds<0){
            printf("epoll_wait failed!%m\n");
            close(epfd);
            close(serverSocket);
            exit(-2);
        }else if(num_of_fds==0){
            continue;
        }else{
            // if server have thing 
            for(int i=0;i<num_of_fds;i++){
                if(serverSocket == events[i].data.fd){
                    int cfd = accept(serverSocket, &add_cliect, &len_cliect);
                    if(cfd==-1){
                        printf("server failed!\n");
                        close(epfd);
                        close(serverSocket);
                        exit(-2);
                    }
                    printf("Cliect connect! ip:%s, cid: %d\n", inet_ntoa(add_cliect.sin_addr), cfd);
                    // register cliect
                    ev.data.fd = cfd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                }else if(events[i].events & EPOLLIN){
                    // if Cliect send data
                    int curFd = events[i].data.fd;
                    int r = recv(curFd, buffer, 1023, 0);
                    if(r>0){
                        buffer[r] = 0;
                        printf("cliect cid: %d: >>>> %s\n", curFd, buffer);
                    }else{
                        printf("cliect leave: id--->%d", curFd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curFd, &events[i]);
                    }
                }
            }
        }
        
    }
    for (int i = 0; i < NUM; i++)
    {
        if (-1 == events[i].data.fd)
        {
            close(events[i].data.fd);
            events[i].data.fd = -1;
        }
    }
    printf("bye!\n");
    return 0;
}
