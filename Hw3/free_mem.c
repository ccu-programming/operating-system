#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
    int i, j;
    int input = atoi(argv[argc - 1]);

    printf("pid = %d\n", getpid());
    getchar();

    mlockall(MCL_FUTURE);
    
    char **ptr = (char **)malloc(sizeof(char *) * input);
    for(i = 0; i < input; i++){
        ptr[i] = (char *)malloc(1024 * 1024 * 1024);
        for(j = 0; j < 1024 * 1024 * 1024; j++){
            ptr[i][j] = 0;
        }
    }
    printf("malloc memory\n");
    getchar();

    for(i = 0; i < input; i++){
        free(ptr[i]);
    }
    free(ptr);
    printf("free memory\n");
    getchar();
}
