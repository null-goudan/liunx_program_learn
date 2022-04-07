#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#define NUM 100

int serverSocket;

int cliectFds[100]={0};

void exit_handler()
{
    for (int i = 0; i < NUM-1; i++)
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
    fd_set fds;
    int maxFd = 0;

    for (int i = 0; i < NUM; i++)
    {
        cliectFds[i] = -1;
    }
    struct sockaddr_in add_cliect = {0};
    int len_cliect = sizeof(add_cliect);
    char buffer[256];

    while (1)
    {
        FD_ZERO(&fds);
        maxFd = (maxFd > serverSocket) ? maxFd : serverSocket;
        FD_SET(serverSocket, &fds);
        // Put the client's file description symbol into the description symbol set
        for (int i = 0; i < NUM; i++)
        {
            if (-1 != cliectFds[i])
            {
                FD_SET(cliectFds[i], &fds);
            }
        }
        int r = select(maxFd + 1, &fds, NULL, NULL, NULL);
        if (-1 == r)
        {
            printf("server failed!\n");
            close(serverSocket);
            exit(-1);
        }
        else if (0 == r)
        {
            printf("waiting....\n");
            continue;
        }
        else
        {
            // is server have something
            if (FD_ISSET(serverSocket, &fds))
            {
                // have something , Cliect is connect
                int cliectFd = accept(serverSocket, &add_cliect, &len_cliect);
                if (cliectFd == -1)
                {
                    printf("accept failed!\n");
                    continue;
                }
                else
                {
                    printf("1111");
                    printf("There is Cliect connected! id: %d\n", cliectFd);
                    // save cliect's File description symbol
                    for (int i = 0; i < NUM; i++)
                    {
                        if (-1 == cliectFds[i])
                        {
                            cliectFds[i] = cliectFd;
                            maxFd = (maxFd > cliectFd) ? maxFd : cliectFd;
                            break;
                        }
                    }
                }
            }
            for (int i = 0; i < NUM; i++)
            {
                if (-1 != cliectFds[i] && FD_ISSET(cliectFds[i], &fds))
                {
                    int rc = recv(cliectFds[i], buffer, 1024, 0);
                    if (rc > 0)
                    {
                        buffer[rc] = 0;
                        printf("id:%d: %s\n",cliectFds[i], buffer);
                    }
                    else
                    {
                        printf("There is cliect disconnected!\n");
                        cliectFds[i] = -1;
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
