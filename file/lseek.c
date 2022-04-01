#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd = open("lseek.txt", O_RDONLY);
    if(-1 == fd){
        perror("don't open the file!");
        exit(-1);
    }
    printf("open the file successfully!\n");
    printf("lseek result:\n");
    while(1){
        char ch;
        int r = read(fd, &ch, 1);
        if(r>0){
            lseek(fd, 2, SEEK_CUR);
        }else{
            break;
        }
        printf("%c\t", ch);
    }
    close(fd);
    return 0;
}