#include<stdio.h>
#include<fcntl.h>
#include <unistd.h> 
#include <stdlib.h> //  system fuctions as exit

int main(){
    int fd;
    fd = open("file_open.txt", O_RDWR);
    if(-1 == fd){
        // printf("don't open the file : %m\n")
        // perror("don't open the file");   // error output function while output the error infomation at the words end
                                            // as the last line
        // return -1;
        fd = open("file_open.txt", O_CREAT | O_WRONLY);
        if(-1 == fd){
            perror("can't create the file !");
            exit(-1);
        }
        printf("create the file successful!");
        close(fd); // clone file function
        fd = open("file_open.txt", O_RDWR);
    }
    printf("open the file successful!\n");
    close(fd);
    return 0;
}
