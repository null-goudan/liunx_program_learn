#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(){
    // read pipe data:

    // open the pipe
    int fd = open("test.pipe", O_RDWR);
    if(-1 == fd) printf("open the pipe failed!: %d"), unlink("test.pipe"), exit(-1);
    printf("pipe opened!\n");
    // read data from pipe file by loop
    char buffer [1024] = {0};
    int n = 0;
    while(1){
        int rd = read(fd, buffer, 1024);
        if(rd>0){
            printf("%s\n", buffer);
        }
    }
    // close the file 
    close(fd);
}