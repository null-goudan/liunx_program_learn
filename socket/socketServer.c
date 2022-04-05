#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int serverSocket, clientSocket;

// struct sockaddr_in {
//     short int sin_family; /* 地址族，AF_xxx 在socket编程中只能是AF_INET */
//     unsigned short int sin_port; /* 端口号 （使用网络字节顺序） */
//     struct in_addr sin_addr; /* 存储IP地址 4字节 */
//     unsigned char sin_zero[8]; /* 总共8个字节，实际上没有什么用，只是为了和struct sockaddr保持一样的长度 */
// };
// struct in_addr{
//     in_addr_t s_addr;
// };

void exit_handler(){
    close(clientSocket);
    close(serverSocket);
    exit(0);
    printf("bye!\n");
}

int main(int argc, char* argv[]){
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

    struct sockaddr_in add_cliect = {0};
    int len_cliect = sizeof(add_cliect);

    // accept client connect
    clientSocket = accept(serverSocket,(struct sockaddr_in *)&add_cliect, &len_cliect);
    if(-1==clientSocket) printf("server failed!%m\n"),exit(-1);
    printf("Cliect connect!\nClient %s\n", inet_ntoa(add_cliect.sin_addr));

    //get msg
    char buffer[1024];
    while(1){
        int r = recv(clientSocket, buffer, 1024, 0);
        if(r>0){
            buffer[r] = 0;
            printf(">> %s\n", buffer);
        }
    }
    close(clientSocket);
    close(serverSocket);
    printf("bye!\n");
    return 0;
}

