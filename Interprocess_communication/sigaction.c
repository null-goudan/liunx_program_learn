#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

void handle(){
    printf("shit! mother fuck! don't close me!\n");
}

void handler(int n, siginfo_t* info, void * arg){
    printf("higher handler of signal!:\n");
    // printf("n: %d, msg: %d\n", n, info->si_int);

    // accept a address and use 
    printf("n: %d, msg: address : %p", info->si_ptr);
    int(*p)(int, int) = info->si_ptr;
    printf("max of (2, 3) is : %d", (*p)(2, 3));
    return ;
}

int main(){
    struct sigaction act = {0};
    struct sigaction oldact = {0};
    act.sa_handler = handle;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    printf("this process' id is %d\n", getpid());
    sigaction(2, &act , &oldact);
    int n = 0;
    while(1){
        printf("n:%d\n", n++);
        sleep(1);
    }
    // commond : kill -s SIGINT 137165  will output: shit! mother fuck! don't close me!
    return 0;
}