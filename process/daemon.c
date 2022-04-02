#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

int main(){
    // 1. Reset file permissions
    umask(0);
    // 2. Create a child process
    int rnt = fork();
    
    if(rnt < 0) printf("create process failed!"), exit(-1);
    else if(rnt>0) printf("parent process end!"), exit(0); // 3. Let the parent process end
    else if(rnt==0){
        // process start!
        printf("PID: %d\n", getpid());
        // 4. Create a new session
        setsid();
        // 5. Prevents child processes from becoming zombie processes, Ignore the signal: SIGCHLD and SIGHUP
        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);
        // 6. Change the working directory to the root dir
        chdir("/");
        // 7. Redirect file descriptors
        int fd = open("/dev/NULL", O_RDWR);
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
    }
    // Create the daemon successfully

    // The daemon start here! It's an endless loop
    while(1){
        sleep(1);
    }
}

