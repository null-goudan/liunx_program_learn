#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int clientSocket;

void exit_handler(){
    close(clientSocket);
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
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == clientSocket) printf("don't get clientSocket!%m\n"),exit(-1);
    printf("Get the clientSocket!\n");

    // get server protrol address family
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(argv[1]);
    add.sin_port = atoi(argv[2]);

    //get msg
    char buffer[256];
    char temp[256];
    int len = sizeof(add);
    while(1){
        scanf("%s", buffer);
        int s = sendto(clientSocket, buffer, sizeof(buffer), 0, &add, sizeof(add));
        if(s == -1) printf("%m\n");
        int r = recvfrom(clientSocket, temp, 256, 0, &add, &len);
        if(r>0){
            printf("server: \t %s\n", temp);
        }
    }
    close(clientSocket);
    printf("bye!\n");
    return 0;
}

