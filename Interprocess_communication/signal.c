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

int main(){
    signal(SIGINT, handle);
    printf("pid: %d\n", getpid());
    int n = 0;
    while(1){
        printf("n:%d\n", n++);
        sleep(1);
    }
    // commond : kill -s SIGINT 137165  will output: shit! mother fuck! don't close me!
    return 0;
}
