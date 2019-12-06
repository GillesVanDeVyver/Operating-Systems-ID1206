#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "green.h"

green_cond_t cond;
green_mutex_t mutex;
pthread_cond_t emptyP, fullP;
pthread_mutex_t mutexP;
int numThreads = 2;

int buffer, productions;
green_cond_t empty, full;
void produce()
{
    for (int i = 0; i < productions / (numThreads / 2); i++)
    {
        green_mutex_lock(&mutex);
        while (buffer == 1) //wait for consumer before producing more
            green_cond_wait(&empty, &mutex);
        buffer = 1;
        //printf("Produced!\n");
        green_cond_signal(&full);
        green_mutex_unlock(&mutex);
    }
}

void consume()
{

    for (int i = 0; i < productions / (numThreads / 2); i++)
    {
        green_mutex_lock(&mutex);
        while (buffer == 0) //wait for producing before consuming
            green_cond_wait(&empty, &mutex);
        buffer = 0;
        //printf("Consumed!\n");
        green_cond_signal(&empty);
        green_mutex_unlock(&mutex);
    }
}

void produceP()
{

    for (int i = 0; i < productions / (numThreads / 2); i++)
    {
        pthread_mutex_lock(&mutexP);
        while (buffer == 1) //wait for consumer before producing more
            pthread_cond_wait(&empty, &mutex);
        buffer = 1;
        //printf("Produced!\n");
        green_cond_signal(&fullP);
        green_mutex_unlock(&mutexP);
    }
}

void consumeP()
{

    for (int i = 0; i < productions / (numThreads / 2); i++)
    {
        pthread_mutex_lock(&mutex);
        while (buffer == 0) //wait for producer before consuming
            green_cond_wait(&empty, &mutex);
        buffer = 0;
        //printf("Consumed!\n");
        green_cond_signal(&emptyP);
        green_mutex_unlock(&mutexP);
    }
}

void *testConsumerProducer(void *arg)
{

    int id = *(int *)arg;
    if (id % 2 == 0)
    {
        produce();
    }
    else
    {
        consume();
    }
}

void *testConsumerProducerP(void *arg)
{

    int id = *(int *)arg;
    if (id % 2 == 0)
    {
        produceP();
    }
    else
    {
        consumeP();
    }
}

void testGreen(int *args)
{
    green_t threads[numThreads];

    for (int i = 0; i < numThreads; i++)
        green_create(&threads[i], testConsumerProducer, &args[i]);

    for (int i = 0; i < numThreads; i++)
        green_join(&threads[i]);
}

void testPthread(int *args)
{

    pthread_t threads[numThreads];

    for (int i = 0; i < numThreads; i++)
        pthread_create(&threads[i], NULL, testConsumerProducerP, &args[i]);

    for (int i = 0; i < numThreads; i++)
        pthread_join(&threads[i], NULL);
}

int main() {

    clock_t c_start, c_stop;
    double processTimeGreen = 0, processTimeP = 0;
    green_cond_init(&cond);
    green_cond_init(&empty);
    green_cond_init(&full);
    green_mutex_init(&mutex);

    pthread_cond_init(&fullP, NULL);
    pthread_cond_init(&emptyP, NULL);
    pthread_cond_init(&mutexP, NULL);

    printf("#Benchmark, creating and producing/consuming with threads!\n#\n#\n");
    printf("#{#productions\ttimeGreen(ms)\ttimePthread(ms)}\n");

    int numRuns = 10;
    for(int run = 1; run <= numRuns; run++) {
        buffer = 0;
        productions = 1000 * 2 * run;

        int args [numThreads];
        for(int i = 0; i < numThreads; i++)
            args[i] = i;

        c_start = clock();
        testGreen(args);
        c_stop = clock();
        processTimeGreen = ((double)(c_stop-c_start)) / ((double)CLOCKS_PER_SEC/1000);
        c_start = clock();
        testPthread(args);
        c_stop = clock();
        processTimeP = ((double)(c_stop-c_start)) / ((double)CLOCKS_PER_SEC/1000);
        printf("%d\t%f\t%f\n", productions, processTimeGreen, processTimeP);    
    }    

    printf("done\n");
}