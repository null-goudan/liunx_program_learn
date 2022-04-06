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

int serverSocket, clientSocket;

int main(){
    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == serverSocket) printf("don't get serverSocket_Id!%m\n"),exit(-1);
    printf("Get the serverSocket!\n");

    // create server protrol address family
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr("192.168.214.128");
    add.sin_port = 10086;

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
    clientSocket = accept(serverSocket,(struct sockaddr_in *)&add_cliect, (struct socklen_t*)&len_cliect);
    if(-1==clientSocket) printf("server failed!%m\n"),exit(-1);
    printf("Cliect connect!\nClient %s\n", inet_ntoa(add_cliect.sin_addr));

    //get msg
    char buffer[1024];
    char filename[256]={0};
    int filesize;
    // recive the file name
    r = recv(clientSocket, filename, 255, 0);
    if(r>0){
        printf("file name:\t %s \n", filename);
    }
    // recive the file size
    r = recv(clientSocket, &filesize, 4, 0);
    if(r==4){
        printf("file size:\t %d byte\n", filesize);
    }

    int cnt = 0;
    // create file 
    int fd = open(filename, O_CREAT|O_WRONLY, 0666);
    while(1){
        r = recv(clientSocket, buffer, 1024, 0);
        if(r>0){
            write(fd, buffer, r);
            cnt+=r;
            if(cnt>=filesize) break;
        }
    }
    close(fd);
    printf("recv file successful!\n");
    sleep(1);
    close(clientSocket);
    close(serverSocket);
    printf("bye!\n");
    return 0;
}

