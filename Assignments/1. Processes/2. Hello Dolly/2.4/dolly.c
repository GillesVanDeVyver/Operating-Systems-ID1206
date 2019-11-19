#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int x = 123, pid = fork();

    if (pid == 0) {
        printf("Child : x is %d and the adress is %p \n", x, &x);
        x = 42; 
        sleep(1);
        printf("Child x is %d and the adress is %p \n", x, &x);   
    } else {
        printf("Mother: x is %d and the adress is %p \n", x, &x);
        x = 13;
        sleep(1);
        printf("Mother: x is %d and the adress is %p \n", x, &x);
        wait(NULL);
    }
    return 0;
}