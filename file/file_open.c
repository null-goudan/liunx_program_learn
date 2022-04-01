#include<stdio.h>
#include<fcntl.h>

int main(){
    int fd;
    fd = open("file_open.txt", O_RDWR);
    if(-1 == fd){
        printf("don't open the file: %m\n");
    }
    else {
        printf("open the file successful!\n");
    }
}
