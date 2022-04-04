#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int func(int a, int b){
    return a>b?a:b;
}


int main(int argc, char* argv[]){
    int pid = atoi(argv[1]);
    int sig = atoi(argv[2]);
    union sigval sigv; 
    // sigv.sival_int = 837471654;
    // sigqueue(pid, sig, sigv);
    //
    // send a address
    sigv.sival_ptr = func;
    sigqueue(pid, sig, sigv); 
    return 0;
}
