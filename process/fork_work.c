#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>

int main(){
    int fd = open("ctrl.dat", O_RDWR);
    if(-1 == fd){
        printf("don't open the file: %m");
        exit(-1);
    }
    printf("open the file successfully!\n");

    ftruncate(fd, sizeof(int));

    int* ctrl = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    int exit_flag = 0;

    int rnt = fork();
    if(rnt){
        // parant process
        while(1){
            char ch = getchar();
            if(ch == ' '){
                *ctrl = 1;
            }else if(ch == '1'){
                *ctrl = 0;
            }
            else if(ch == '2'){
                break;
            }
        }
        wait(0);
    }else{
        // child process
        while(1){
            time_t tt = time(NULL);
            struct tm* now_time = localtime(&tt);
            printf("now time: %02d : %02d : %02d \n", now_time->tm_hour, now_time->tm_min, now_time->tm_sec);
            sleep(1);
            while(*ctrl);
        }
    }
    return 0;
}


