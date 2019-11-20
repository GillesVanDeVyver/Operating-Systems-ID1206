#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ptmall.h"
#include "rand.h"

int allocs; // number of blocks requested
int buffer; //how many blocks we can hold at a given time
int maxReqSize; // max block size request

/**
* Will palloc/pree simulating a real program
*/
void testAlloc(int bufferSize){
  void *buffer[bufferSize];
  for(int i = 0; i < bufferSize; i++){
    buffer[i] = NULL;
  }

  for(int i = 0; i < allocs; i++){
    int index = rand() % bufferSize;
    if(buffer[index] != NULL){
      pree(buffer[index]);
    }

    size_t size = (size_t)request(maxReqSize);
    int *memory;
    memory = palloc(size);

    if(memory == NULL){
      fprintf(stderr, "palloc failed\n");
      return;
    }
    buffer[index] = memory;
    /* writing to the memory so we know it exists */
    *memory = 123;
  }
}

void evalFlistLength(){
  printf("# Checking length of flist\n# BufferSize\tflistLength\n");
  for(int i = 10; i < buffer; i+= 10){
    init();
    testAlloc(i);
    printCountLengthOfFlist(i);
    terminate();
  }
  init();
  testAlloc(buffer);
  printCountLengthOfFlist(buffer);
  terminate();
}

void evalFlistDistr(){
  printf("# Checking distribution of the block sizes in flist\n");
  init();
  testAlloc(buffer);
  printSizeDistributionOfFlist(buffer);
  terminate();
}

void evalTimePerformance(){
  printf("# Evaluation of time performance\n# BufferSize\tTime(ms)\n");
  clock_t time_start, time_stop;
  double timeAlloc = 0;
  for(int i = 10; i < buffer; i+= 10){
    init();
    time_start = clock();
    testAlloc(i);
    time_stop = clock();
    terminate();
    timeAlloc = ((double)(time_stop - time_start)) / ((double)CLOCKS_PER_SEC/1000);
    printf("%d\t%f\n", i, timeAlloc);
  }
  init();
  time_start = clock();
  testAlloc(buffer);
  time_stop = clock();
  terminate();
  timeAlloc = ((double)(time_stop - time_start)) / ((double)CLOCKS_PER_SEC/1000);
  printf("%d\t%f\n", buffer, timeAlloc);
}

void testSanity(){
  init();
  testAlloc(buffer);
  sanity();
  terminate();
}

int main(int argc, char const *argv[]){
  if(argc < 4){
    printf("Please enter:\tBuffer size\tLoops\tMax size of request\n");
    exit(1);
  }
  buffer = atoi(argv[1]);
  allocs = atoi(argv[2]);
  maxReqSize = atoi(argv[3]);
  srand(time(0));
  //evalFlistLength();
  //evalFlistDistr();
  evalTimePerformance();
  //testSanity();
  return 0;
}
