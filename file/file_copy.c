#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    # if 0
    // this is the system command method
    char buffer[256] = {0};
    sprintf(buffer, "cp %s %s", argv[1], argv[2]);
    printf("%s", buffer);
    system(buffer);
    // /file_copy file_copy.c file_copy.txt
    return 0;
    # else
    // this is the c library method
    int fdFst, fdSnd;
    char buffer [1024] = "";
    fdFst = open(argv[1], O_RDONLY);
    if(-1 == fdFst){
        perror("don't open first file!");
        exit(-1);
    }
    fdSnd = open(argv[2], O_WRONLY|O_CREAT);
    while(1){
        int rd = read(fdFst, buffer, 1024);
        if(rd>0){
            write(fdSnd, buffer, 1024);
        }else{
            break;
        }
    }
    close(fdFst);
    close(fdSnd);
    return 0;
    # endif
}

