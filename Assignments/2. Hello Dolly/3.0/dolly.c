#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    
    int pid = fork();
    
    if (pid == 0) {
        printf("I'm the child %d with parent %d \n", getpid(), getppid());
    } else {
        printf("I'm the parent %d with parent %d\n", getpid(), getppid());
        wait(NULL);
    }
    return 0;

}