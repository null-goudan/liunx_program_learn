#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <string.h>

int clientSocket;


int main(int argc, char* argv[]){
    // create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == clientSocket) printf("don't get clientSocket!%m\n"),exit(-1);
    printf("Get the clientSocket!\n");

    // get server protrol address family
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr("192.168.214.128");
    add.sin_port = 10086;

    // client connect
    int r = connect(clientSocket, (struct sockaddr_in *)&add, sizeof(add));
    if(-1==r) printf("server failed!%m\n"),close(clientSocket),exit(-1);
    printf("Connect!\n");

    
    // open the file
    int fd = open(argv[1], O_RDONLY);
    if(-1==fd) printf("open file failed!%m\n"),close(clientSocket),exit(-1);
    printf("start send file!\n");

    // get filesize
    int filesize = 0;
    struct stat st={0};
    stat(argv[1], &st);
    printf("size of file: %d bytes\n", st.st_size);
    filesize = st.st_size;

    // send msg
    send(clientSocket, argv[1], strlen(argv[1]), 0);
    send(clientSocket, (char *)&filesize, 4, 0);
    char buffer[1024];
    while(1){
        int r = read(fd, buffer, 1024);
        if(r>0){
            send(clientSocket, buffer, r, 0);
        }else{
            break;
        }
    }
    close(fd);
    printf("send finished!\n");
    sleep(2);
    close(clientSocket);
    printf("bye!\n");
    return 0;
}

