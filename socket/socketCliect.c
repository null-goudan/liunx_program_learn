#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int clientSocket;

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
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientSocket) printf("don't get clientSocket!%m\n"),exit(-1);
    printf("Get the clientSocket!\n");

    // get server protrol address family
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(argv[1]);
    add.sin_port = atoi(argv[2]);


    // client connect
    clientSocket = connect(clientSocket, (struct sockaddr_in *)&add, sizeof(add));
    if(-1==clientSocket) printf("server failed!%m\n"),exit(-1);
    printf("Connect!");

    //get msg
    char buffer[1024];
    while(1){
        scanf("%s", buffer);
        send(clientSocket, buffer, sizeof(buffer), 0);
    }
    close(clientSocket);
    printf("bye!\n");
    return 0;
}

