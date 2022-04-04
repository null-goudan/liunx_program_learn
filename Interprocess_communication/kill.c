#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


// int atoi(char* str){
//     int num =0 ;
//     while(1){
//         num += *str - '0';
//         str++;
//         if(*str=='\0') break;
//         num *= 10;
//     }
// }

int main(int argc, char* argv[]){
    int pid = atoi(argv[1]);
    int sig = atoi(argv[2]);
    printf("pid: %d", pid);
    kill(pid, sig);
    return 0;
}
