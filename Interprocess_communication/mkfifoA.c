#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(){
    // make a fifo
    int r = mkfifo("test.pipe", 0666);
    if(-1==r) printf("create pipe failed!:%m"), exit(-1);
    printf("pipe started!\n");
    // open the pipe
    int fd = open("test.pipe", O_RDWR);
    if(-1 == fd) printf("open the pipe failed!: %d"), unlink("test.pipe"), exit(-1);
    printf("pipe opened!\n");
    // write into pipe file loop
    char buffer [1024] = {0};
    int n = 0;
    while(1){
        sprintf(buffer , "yyds : goudan -> %d", n++);
        write(fd, buffer, 1024);
        sleep(1);
    }
    // close the file 
    close(fd);
    // delete the pipe file
    unlink("test.pipe");
}