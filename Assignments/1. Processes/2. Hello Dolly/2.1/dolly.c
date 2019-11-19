#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int pid = fork();

    if (pid == 0) {
        printf("I'm the child process %d\n", getpid());
        sleep(1);
    } else {
        printf("My child is called %d\n", pid);
        wait(NULL);
        printf("My child has terminated \n");
    }
    printf("This is the end (%d)\n", getpid());
    return 0;
}