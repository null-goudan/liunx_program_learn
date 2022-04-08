#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <string.h>

int main(){
    struct pollfd fds[10];
    int now_num = 0;
    now_num += 1;
    fds[0].events =  POLLIN;
    fds[0].fd = 0;

    int r;
    char buffer[1024];
    while(1){
        r = poll(fds, now_num, 0);
        if(r < 0 ){
            printf("error : %m\n");
            exit(-1);
        }else if(r==0){
            continue;;
        }else{
            printf("There is something doing in the file_id: %d\n", fds[0].fd);
            if(fds[0].revents && POLLIN){
                memset(buffer, 0, sizeof(buffer));
                scanf("%s", buffer);
                printf(">> %s\n", buffer);
            }
        }
    }
    return 0;
}

