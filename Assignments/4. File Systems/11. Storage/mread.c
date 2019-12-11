#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define BLOCKS 1024*1024
#define SIZE 512
#define STEP 64
#define COUNT 100000
#define LOOP 100 // can not be 0
#define RES 10

int compare(const void *a, const void *b) {
  return (int)(*(long *)a - *(long*)b);
}

long n_sec(struct timespec *start, struct timespec *stop) {
  long diff_sec = stop->tv_sec - start->tv_sec;
  long diff_nsec = stop->tv_nsec - start->tv_nsec;
  long wall_nsec = (diff_sec * 1000000000) + diff_nsec;
  return wall_nsec;
}
int main(int argc, char *argv[]) {
  if(argc < 2) {
      printf("usage: generate <file name>\n");
      return -1;
  }
  char *name = argv[1];

  int *heap = malloc(sizeof(int)*BLOCKS*SIZE);

  /* Test removing this for-loop */
  for(int b =0; b < BLOCKS; b++) {
    heap[b*SIZE] = b;
  }

  int entries = SIZE / STEP;

  long **table = malloc(sizeof(long*) * entries);

  for(int i = 0; i < entries; i++) {
      table[i] = malloc(sizeof(long) * COUNT);
  }

  for(int c = 0; c < COUNT; c++) {
      int b = rand() % BLOCKS;

      for(int e = 0; e < entries; e++) {
          struct timespec t_start, t_stop;
          int buffer;
          clock_gettime(CLOCK_MONOTONIC, &t_start);
          for(int l = 0; l < LOOP; l++) {
            buffer = buffer + heap[b*SIZE+e+l];
          }
          clock_gettime(CLOCK_MONOTONIC, &t_stop);
          table[e][c] = ((n_sec(&t_start, &t_stop) - RES) / LOOP);
      }
  }

  printf("#N\tMin\tQ1\tMed\tQ3\tD9\tMax\n");
  for(int e = 0; e < entries; e++) {
      qsort(table[e], COUNT, sizeof(long), compare);
      long min = table[e][0];
      long q1 = table[e][COUNT/4];
      long med = table[e][COUNT/2];
      long q3 = table[e][3*(COUNT/4)];
      long d9 = table[e][9*(COUNT/10)];
      long max = table[e][COUNT - 1];
      printf("%d\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", e*STEP, min, q1, med, q3, d9, max);
  }

  printf("done\n");
  return 0;
}