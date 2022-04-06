#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int serverSocket;

void exit_handler(){
    close(serverSocket);
    printf("bye!\n");
    exit(0);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("don't have enough arg!\n");
        exit(0);
    }
    // signal exit()
    signal(2, exit_handler);
    // create socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
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

    struct sockaddr_in add_cliect = {0};
    int len_cliect = sizeof(add_cliect);

    //get msg
    char buffer[256];
    while(1){
        int r = recvfrom(serverSocket, buffer, 255, 0, &add_cliect, &len_cliect);
        if(r>0){
            buffer[r] = 0;
            printf(">> %s\n", buffer);
            sendto(serverSocket, "I'm yyx", strlen("I'm yyx"), 0, &add_cliect, sizeof(add_cliect));
        }
    }
    close(serverSocket);
    printf("bye!\n");
    return 0;
}

