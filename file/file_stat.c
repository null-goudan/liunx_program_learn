#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    struct stat st = {0};
    int res = stat(argv[1], &st);
    if(res){
        printf("don't get the file: %s mode!", argv[1]);
        exit(-1);
    }
    else{
        if(st.st_mode & S_IFDIR){
            printf("dir file");
        }else{
            printf("normal file");
        }
    }
}
