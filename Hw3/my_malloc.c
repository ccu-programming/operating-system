#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(){
    int memory_size = 1024 * 1024 * 40;

    printf("pid = %d\n", getpid());
    getchar();

    char *ptr = (char *)malloc(memory_size);
    printf("malloc memory\n");  
    getchar();

    int i = 0;
    while(++i < memory_size){
        *ptr = 0;
        ptr++;
    }
    printf("access memory\n");
    getchar();
}


