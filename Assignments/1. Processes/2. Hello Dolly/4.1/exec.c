#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int pid = getpid();
    if (pid == 0) {
        execlp("ls", "ls", NULL);
        printf("This will only happen if exec fails\n");
    }   else {
        wait(NULL);
        printf("We're done\n");
    }
    return 0;
}

