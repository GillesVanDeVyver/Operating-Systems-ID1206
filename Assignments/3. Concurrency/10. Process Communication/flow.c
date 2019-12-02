#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#define ITERATIONS 10
#define BURSTS 10000000

int main () {
    int descr[2];

    assert(0 == pipe(descr));
    
    if(fork() == 0) {
        //consumer
        for(int i = 0; i < ITERATIONS; i++) {
            for(int j = 0; j < BURSTS; j++) {
                int buffer[10];
                read(descr[0], &buffer, 10);
            }
        sleep(1);
        }
        printf("cosumer done\n");
        return 0;
    }

    //producer
    for(int i = 0; i < ITERATIONS; i++) {
        for(int j = 0; j < BURSTS; j++) {
            write(descr[0], "0123456789", 10);
        }
    printf("producer burst %d done\n",i);
    }   
    wait(NULL);
    printf("all done\n");
}


