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
    printf("open the file successfully!");

    // notice : writing into file need charge the size of file !

    // charge the length of file
    ftruncate(fd, 3*sizeof(struct student));
    // map virtual memory
    struct student* p = mmap(NULL, 3*sizeof(struct student), PROT_WRITE,MAP_SHARED,fd, 0);

    if((void*)-1 == p) printf("don't map memory:%m"), exit(-1);
    printf("map memory successfully!");
    // using virtual memory
    struct student* pp = p;

    pp->id = 1;
    strcpy(p->name, "Yang");
    pp->score = 99.99;
    pp++;
    pp->id = 2;
    strcpy(p->name, "Wang");
    pp->score = 99.99;
    pp++;
    pp->id = 3;
    strcpy(p->name, "Xu");
    pp->score = 99.99;

    // delete the mapped virtual memory
    munmap(p, 3*sizeof(struct student));
    close(fd);
}