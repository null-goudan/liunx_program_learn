#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/poll.h>

#define NUM 100

int serverSocket;

int cliectFds[100]={0};

void exit_handler()
{
    for (int i = 0; i < NUM; i++)
    {
        if (-1 == cliectFds[i])
        {
            close(cliectFds[i]);
            cliectFds[i] = -1;
        }
    }
    close(serverSocket);
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

    struct pollfd pfds[NUM];
    int num_of_fds=0;
    // first put the socket of server into the array;
    pfds[0].fd = serverSocket;
    pfds[0].events = POLLIN;
    num_of_fds++;

    while (1)
    {   
        r = poll(pfds, num_of_fds, 0);
        if(r<0){
            printf("poll failed!%m");
            continue;
        }else if(r==0){
            continue;
        }else{
            // if server have thing 
            if(POLLIN & pfds[0].revents){
                // have cliect connect --> accept
                int cfd = accept(serverSocket, &add_cliect, &len_cliect);
                if(cfd<0){
                    printf("Have something wrong with server %m\n");
                    continue;
                }
                pfds[num_of_fds].fd = cfd;
                pfds[num_of_fds].events = POLLIN;
                num_of_fds++;
                printf("now cliect list: \n");
                for(int i =0;i<num_of_fds;i++){
                    if(pfds[i].fd != -1){
                        printf("id: %d\n", pfds[i].fd);
                    }
                }
                printf("Cliect connected! id ---> %d -> ", cfd);
                printf("Cliect ip address: %s\n", inet_ntoa(add_cliect.sin_addr));
            }
            for(int i = 1;i<=num_of_fds; i++){
                if(POLLIN && pfds[i].revents){
                    int r= recv(pfds[i].fd, buffer, 255, 0);
                    if(r>0){
                        printf("cliect_id: %d: %s\n", pfds[i].fd, buffer);
                    }else{
                        printf("cliect leave: id--->%d", pfds[i].fd);
                        pfds[i].fd = -1;
                        num_of_fds--;
                    }
                }
            }
        }
        
    }
    for (int i = 0; i < NUM; i++)
    {
        if (-1 == cliectFds[i])
        {
            close(cliectFds[i]);
            cliectFds[i] = -1;
        }
    }
    close(serverSocket);
    printf("bye!\n");
    return 0;
}
