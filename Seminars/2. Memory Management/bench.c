#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dlmall.h"
#include "rand.h"

#define BUFFER 100 
#define MAX_SIZE 100 

void runAllocs(int allocs){
  void *buffer[BUFFER];
  for(int i = 0; i < BUFFER; i++){
    buffer[i] = NULL;
  }

  for(int i = 0; i < allocs; i++){
    int index = rand() % BUFFER;
    if(buffer[index] != NULL){
      dfree(buffer[index]);
    }

    size_t size = (size_t)request(MAX_SIZE);
    int *memory;
    memory = dalloc(size);

    if(memory == NULL){
      fprintf(stderr, "palloc failed\n");
      return;
    }
    buffer[index] = memory;
    /* writing to the memory so we know it exists */
    *memory = 123;
  }
}

void checkFreeListLength(int allocs){
  printf("# Checking length of flist\n# Allocs\tflistLength\n");
  int sum, average = 0;
  for(int i = BUFFER; i < allocs+1; i+= 10){
    init();
    runAllocs(i);
    sum+= printCountLengthOfFlist(i);
    average = sum/100;
    terminate();
  }
  printf("Average length of free list is: %d \n", average);
}

void checkFreeListDist(int allocs){
  printf("# Checking distribution of the block sizes in flist\n# Average\tflistLength\n");
  init();
  runAllocs(allocs);
  printAverageSizeDistributionOfFlist();
  terminate();
}

void BenchmarkPalloc(int allocs){
  printf("# Evaluation of time performance\n# Allocs\tTime(ms)\n");
  clock_t time_start, time_stop;
  double timeAlloc = 0;
  for(int i = BUFFER; i < allocs+1; i+= 10){
    init();
    time_start = clock();
    runAllocs(i);
    time_stop = clock();
    terminate();
    timeAlloc = ((double)(time_stop - time_start)) / ((double)CLOCKS_PER_SEC/1000);
    printf("%d\t%f ms\n", i, timeAlloc);
  }
}

void testSanity(int allocs){
  init();
  runAllocs(allocs);
  sanity();
  terminate();
}

int main(int argc, char const *argv[]) {

  if(argc < 2){
    printf("Please enter:\tNumberOfAllocs\n");
    exit(1);
  }

  int allocs = atoi(argv[1]);
  BenchmarkPalloc(allocs);
  sleep(1);
  checkFreeListLength(allocs);
  sleep(1);
  checkFreeListDist(allocs);
  sleep(1);
  testSanity(allocs);

  return 0;
}
