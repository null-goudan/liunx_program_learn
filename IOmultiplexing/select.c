#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    fd_set sets;
    FD_ZERO(&sets);
    FD_SET(0, &sets);

    char buffer[1024];
    int r = 0 ;

    while(1){
        int r = select(1, &sets, NULL, NULL, NULL);
        if(r>0){
            printf("r = %d ,have input:\n", r);
            scanf("%s", buffer);
            printf("%s\n", buffer);
        }else{
            printf("r=%d, no inputs\n", r);
        }
    }
    return 0;
}

