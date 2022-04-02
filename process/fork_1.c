#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    printf("this is parent process\n");
    sleep(1);
    printf("create child process!\n");
    int n = 56;
    int ret = fork();
    // at here, split;
    if (ret) // ret is zero when the process is child process while res is pid of parent process when it is parent process
    {
        // parent process
        while (1)
        {
            printf("process of pid: %d  n: %d, ret: %d\n", getpid(), n++, ret);
            sleep(1);
        }
    }else{
        // child process
        while(1){
            printf("process of pid: %d  n: %d, ret: %d ----- \n", getpid(), n++, ret);
            n++;
            sleep(1);
        }
    }
    return 0;
}
