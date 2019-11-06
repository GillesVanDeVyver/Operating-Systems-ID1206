#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int volatile done; 

void handler(int sig, siginfo_t *siginfo, void *context) {
    
    printf("Signal %d was caught\n", sig);
    
    printf("Your UID is %d\n", siginfo->si_uid);
    printf("Your PID is %d\n", siginfo->si_pid);

    done = 1;
}

int main() {

    int pid = getpid();

    struct sigaction sa;

    printf("Ok - let's go - kill me (%d) \n", pid);

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler; 
    sigemptyset(&sa.sa_mask);

    // and now we catch ... FPE signals 
    if(sigaction(SIGINT, &sa, NULL) !=0) {
        return 0;
    }

    while(!done) {

    }

    printf("Told you so\n");

    return 0;
}