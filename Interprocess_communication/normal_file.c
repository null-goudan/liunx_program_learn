#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
# if 1
// parent and child process communication
int main(){
    int fd = open("normal_file.dat", O_RDWR|O_CREAT, 0666);
    if(-1 == fd) printf("don't open the file!: %m"), exit(-1);
    ftruncate(fd, sizeof(int));
    // the aim of closing the file is be helpful to opening file
    close(fd);
    int rnt = fork();
    // parent write data into file 
    // child read date from file and output the data
    if(rnt){
        int n = 0;
        while(1){
            fd = open("normal_file.dat", O_WRONLY);
            write(fd, &n, sizeof(4));
            n++;
            sleep(1);
        }
    }
    else{
        while(1){
            int n =0;
            fd = open("normal_file.dat", O_RDONLY);
            read(fd, &n , sizeof(int));
            printf("%d\n", n);
            sleep(1);
        }
    }
    return 0;
}
# else
// not parent and child process communication
int main(){

}
# endif