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

int main()
{
    int fd = open("file_write.txt", O_RDONLY);
    if (-1 == fd)
    {
        perror("don't open file !");
        exit(-1);
    }
    printf("open file successfully\n");
    struct stu s;
    for (int i = 0; i < 5; i++)
    {
        read(fd, &s, sizeof(struct stu));
        printf("id: %d, name: %s, score: %f \n",
            s.stu_id, s.name , s.score);
    }
    close(fd);
    return 0;
}