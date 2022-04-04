#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


void handle(){
    printf("the basic linux handle function\n");
}

int main(){
    printf("This process's id is %d\n", getpid());
    sigset_t set, old_set;
    sigemptyset(&set);
    
    sigaddset(&set, 2);
    // In five seconds don't set procmask;
    signal(2, handle);
    sleep(5);

    int ret = 0;
    if(1 == sigismember(&set, 2)){
        printf("set sigprocmask...\n");
        ret = sigprocmask(SIG_BLOCK, &set, &old_set);
        if(0 == ret){
            printf("set success!\n");
        }
        else printf("set failed!\n");
    }
    sleep(20);
    if(1 == sigismember(&set, 2)){
        printf("set sigprocmask none...\n");
        ret = sigprocmask(SIG_UNBLOCK, &set, &old_set);
        if(0 == ret){
            printf("set success!\n");
        }
        else printf("set failed!\n");
    }
    sleep(15);
    return 0;
}
