#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
# if 1
// parent and child process communication
int main(){
    int fd = open("mmap_file.dat", O_RDWR|O_CREAT, 0666);
    if(-1 == fd) printf("don't open the file!: %m"), exit(-1);
    // resize the file.
    ftruncate(fd, sizeof(int));
    
    int* p = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    int rnt = fork();
    // parent write data into file 
    // child read date from file and output the data
    if(rnt){
        int n = 0;
        while(1){
            while(*p);
            printf("%d", n++);
            sleep(1);
        }
        wait(0);
    }
    else{
        while(1){
            read(0, p, sizeof(int));
        }
    }
    munmap(p, sizeof(int));
    close(fd);
    return 0;
}
# else
// not parent and child process communication
int main(){
    
}
# endif