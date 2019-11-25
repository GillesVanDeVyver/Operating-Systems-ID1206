#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ptmall.h"
#include "rand.h"

#define BUFFER 100 //how many blocks we can hold at a given time
#define MAX_SIZE 100 // max block size request

void runAllocs(int allocs){
  void *buffer[BUFFER];
  for(int i = 0; i < BUFFER; i++){
    buffer[i] = NULL;
  }

  for(int i = 0; i < allocs; i++){
    int index = rand() % BUFFER;
    if(buffer[index] != NULL){
      pree(buffer[index]);
    }

    size_t size = (size_t)request(MAX_SIZE);
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

void evalFlistLength(int allocs){
  printf("# Checking length of flist\n# Allocs\tflistLength\n");
  for(int i = BUFFER; i < allocs; i+= 10){
    init();
    runAllocs(i);
    printCountLengthOfFlist(i);
    terminate();
  }
  init();
  runAllocs(allocs);
  printCountLengthOfFlist(allocs);
  terminate();
}

void evalFlistDistr(int allocs){
  printf("# Checking distribution of the block sizes in flist\n# Average\tflistLength\n");
  init();
  runAllocs(allocs);
  printAverageSizeDistributionOfFlist();
  terminate();
}

void evalTimePerformance(int allocs){
  printf("# Evaluation of time performance\n# Allocs\tTime(ms)\n");
  clock_t time_start, time_stop;
  double timeAlloc = 0;
  for(int i = BUFFER; i < allocs; i+= 10){
    init();
    time_start = clock();
    runAllocs(i);
    time_stop = clock();
    terminate();
    timeAlloc = ((double)(time_stop - time_start)) / ((double)CLOCKS_PER_SEC/1000);
    printf("%d\t%f\n", i, timeAlloc);
  }
  init();
  time_start = clock();
  runAllocs(allocs);
  time_stop = clock();
  terminate();
  timeAlloc = ((double)(time_stop - time_start)) / ((double)CLOCKS_PER_SEC/1000);
  printf("%d\t%f\n", allocs, timeAlloc);
}

void testSanity(){
  init();
  runAllocs(1000);
  sanity();
  terminate();
}

void simpleTest(){
  init();
  int *mem = palloc(80);
  int *mom = palloc(1000);
  pree(mem);
  pree(mom);
  sanity();
  terminate();
}

int main(int argc, char const *argv[]){
  
  
  evalTimePerformance(atoi(argv[1]));
  /*if(argc < 2){
    printf("Please enter:\tNumberOfAllocs\n");
    exit(1);
  }
  int allocs = atoi(argv[1]);
  srand(time(0));
  //evalFlistLength(allocs);
  evalFlistDistr(allocs);*/
  //evalTimePerformance(allocs);
  //testSanity();
  //simpleTest();
  return 0;
}
