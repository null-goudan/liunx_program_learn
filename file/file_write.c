#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct stu
{
    int stu_id;
    char name[32];
    float score;
};


int main(){
    int fd = open("file_write.txt", O_WRONLY); // open file with "write" mode
    if(-1 == fd){
        fd = open("file_write.txt", O_CREAT | O_WRONLY);
        if(-1 == fd){
            perror("can't create the file!");
            exit(-1);
        }
        printf("create the file successful!\n");
        fd = open("file_write.txt", O_WRONLY);
    }
    struct stu  students[5] = {
        {1, "Yang", 99.99},
        {2, "Zhang", 99.99},
        {3, "Xu", 99.99},
        {4, "Ye", 99.99},
        {5, "Liu", 99.99},
    };
    write(fd, students, sizeof(struct stu)*5);
    // for(int i =0;i<5;i++){
    //     write(fd, &students[i], sizeof(struct stu));
    // }
    printf("write infomation into file successful!");
    close(fd);
    return 0;
}