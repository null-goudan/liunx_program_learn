#include <stdio.h>
#include <stdlib.h>


int main(){
    int n=0;
    while(1){
        printf("pid: %d: num: %d", getpid(), n++);
    }
    // kill 2 [pid]
}
