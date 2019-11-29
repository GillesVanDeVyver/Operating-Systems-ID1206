#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

volatile int global = 0;

volatile int count = 0;

typedef struct args
{
    int inc;
    int id;
    volatile int *mutex;
} args;

int futex_wait(volatile int *futexp)
{
    return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
}

void futex_wake(volatile int *futexp)
{
    syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}

// Succession will return 0
int try
    (volatile int *lock)
    {
        return __sync_val_compare_and_swap(lock, 0, 1);
    }

int lock(volatile int *lock)
{
    int susp = 0;
    while (try (lock) != 0)
    {
        susp++;
        futex_wait(lock);
    }
    return susp;
}

void unlock(volatile int *mutex)
{
    *mutex = 0;
    futex_wake(mutex);
}

void *increment(void *arg)
{
    int inc = ((args *)arg)->inc;
    int id = ((args *)arg)->id;
    volatile int *mutex = ((args *)arg)->mutex;
    int spin = 0;
    for (int i = 0; i < inc; i++)
    {
        spin += lock(mutex);
        count++;
        unlock(mutex);
    }
    printf("%d\n", spin);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage Peterson <inc>\n");
        exit(0);
    }

    int inc = atoi(argv[1]);

    pthread_t one_p, two_p;
    args one_args, two_args;

    one_args.inc = inc;
    two_args.inc = inc;

    one_args.id = 0;
    two_args.id = 1;

    one_args.mutex = &global;
    two_args.mutex = &global;

    pthread_create(&one_p, NULL, increment, &one_args);
    pthread_create(&two_p, NULL, increment, &two_args);
    pthread_join(one_p, NULL);
    pthread_join(two_p, NULL);

    printf("result is: %d\n", count);
    return 0;
}