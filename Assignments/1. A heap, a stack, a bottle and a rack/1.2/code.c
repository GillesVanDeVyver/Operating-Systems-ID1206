
#include <stdlib.h> 
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>

char global[] = "This is a global string";

int main() {
    int pid = getpid();
    foo:
    printf("Process identifier: %d\n", pid);
    printf("Global string: %p\n", &global);
    printf("The code: %p\n", &&foo);
    return 0;
}