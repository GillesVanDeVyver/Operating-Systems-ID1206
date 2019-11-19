#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PAGES (16)
#define REFS (1024*1024)

int main(int argc, char *argv[]) {

    clock_t c_start, c_stop;

    printf("#pages\t proc\t sum\n ");

    for(int pages = 1; pages <= PAGES; pages+=1) {
        
        int loops =  REFS / pages;

        c_start = clock();
        
        long sum = 0;

        for(int l = 0; l < loops; l++) {
            for(int p = 0; p < pages; p++) {
                //dummy operation
                sum++;
            }
        }
        c_stop = clock();

        {
            double proc;

            proc = ((double)(c_stop-c_start))/CLOCKS_PER_SEC;
            printf("%d\t %.6f\t %ld\n", pages, proc, sum);
        }
    }
    return 0;
}