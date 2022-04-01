#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

struct student{
    int id;
    char name[32];
    float score;
};
int main(int argc, char* argv[]){
    // open the file
    int fd = open(argv[1], O_RDWR);
    if(-1 == fd){
        printf("don't open the file: %m");
        exit(-1);
    }
    printf("open the file successfully!\n");
    // map virtual memory
    struct student* p = mmap(NULL, 3*sizeof(struct student), PROT_READ,MAP_SHARED,fd, 0);

    if((void*)-1 == p) printf("don't map memory:%m"), exit(-1);
    printf("map memory successfully!\n");
    // using virtual memory
    struct student* pp = p;

    for(int i =0;i<3;i++){
        printf("id: %d, name: %s, score: %f\n", pp->id, pp->name, pp->score);
        pp++;
    }
    // delete the mapped virtual memory
    munmap(p, 3*sizeof(struct student));
    close(fd);
}