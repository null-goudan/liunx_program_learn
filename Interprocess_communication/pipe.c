#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fds[2];

    int rnt = pipe(fds);
    if (rnt == -1)
        printf("create pipe failed:%m"), exit(-1);

    int ret = fork();
    if (ret)
    {
        char buffer[1024] = {0};
        while (1)
        {
            int r =  read(fds[0], &buffer, sizeof(char) * 1024);
            if(r>0){
                buffer[r] = 0;
                printf(">> %s\n", buffer);
            }
        }
    }
    else
    {
        char buffer[1024] = {0};
        while(1){
            scanf("%s", buffer);
            write(fds[1], &buffer, strlen(buffer));
        }
    }
    close(fds[0]);
    close(fds[1]);
}
