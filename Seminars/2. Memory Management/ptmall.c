#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <errno.h>


#define TRUE 1
#define FALSE 0

#define HEAD (sizeof(struct head))
#define MIN(size) (((size) > (8))?(size):(8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct head*)memory - 1)
#define HIDE(block) (void*)((struct head*)block + 1)
#define ALIGN 8
#define ARENA (64*1024)

struct head *arena = NULL;
struct head *flist = NULL;

struct head {
  uint16_t bfree; // 2 bytes, the status of the block before
  uint16_t bsize; // 2 bytes, the size of the block before
  uint16_t free;  // 2 bytes, the status of the block
  uint16_t size;  // 2 bytes, the size (max 2^16)
  struct head *next;  // 8 bytes pointer
  struct head *prev;  // 8 bytes pointer
};

struct head *after(struct head *block){
  return (struct head*)((char *) block + block->size + HEAD);
}

struct head *before(struct head *block){
  return (struct head*)((char *) block - block->bsize - HEAD);
}

/**
* Splits the block and returns the splitted
* block with the size that was requested.
*/
struct head *split(struct head *block, int size){
  int rsize = block->size - size - HEAD;
  block->size = rsize;

  struct head *splt = after(block);
  splt->bsize = block->size;
  splt->bfree = block->free;
  splt->size = size;
  splt->free = FALSE;

  struct head *aft = after(splt);
  aft->bsize = splt->size;

  return splt;
}

/**
* Calls mmap to allocate the arena. Called once...
*/
struct head *new(){
  if(arena != NULL){
    printf("one arena already allocated \n");
    return NULL;
  }

  // using mmap, but could have used sbrk
  struct head *new = mmap(NULL, ARENA,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if(new == MAP_FAILED){
    printf("mmap failed: error %d\n", errno);
    return NULL;
  }

  /* make room for head and sentinel */
  unsigned int size = ARENA - 2*HEAD;

  new->bfree = FALSE;
  new->bsize = 0;
  new->free = TRUE;
  new->size = size;

  struct head *sentinel = after(new);

  /* only touch the status fields */
  sentinel->bfree = new->free;
  sentinel->bsize = size;
  sentinel->free = FALSE;
  sentinel->size = 0;

  /* this is the only arena we have */
  arena = (struct head*)new;
  return new;
}

/**
* Detachs the block from the flist.
*/
void detach(struct head *block){
  if(block->next != NULL){
    block->next->prev = block->prev;
    //block->prev = NULL;
  }

  if(block->prev != NULL){
    block->prev->next = block->next;
    //block->next = NULL;
  }

  if(block == flist){
    flist = flist->next;
  }
}

/**
* Inserts the block to the beginning of flist.
*/
void insert(struct head *block){
  block->next = flist;
  block->prev = NULL;
  if(flist != NULL){
    flist->prev = block;
  }
  flist = block;
}

/**
* Insert alá insertion sort
*/
void insert_order(struct head *block){
  struct head *ptr = flist;
  if(ptr == NULL){
    block->prev = NULL;
    block->next = NULL;
    flist = block;
    return;
  }
  struct head *prevPtr = ptr->prev;
  while(ptr != NULL){
    if(block->size <= ptr->size){
      if(prevPtr != NULL){
        prevPtr->next = block;
      }
      block->prev = prevPtr;
      ptr->prev = block;
      block->next = ptr;

      if(ptr == flist){
        flist = block;
      }
      return;
    }
    prevPtr = ptr;
    ptr = ptr->next;
  }

  block->next = NULL;
  prevPtr->next = block;
  block->prev = prevPtr;
}

/**
* Adjust the request so it is bigger than MIN (8)
* and also a multiple of 8.
*/
int adjust(int request){
  int min = MIN(request);
  int rem = min % ALIGN;
  if(rem == 0){
    return min;
  }
  return min + ALIGN - rem;
}

/**
* Tries to find a block of the given size
* and if found check if it is possible to split.
*/
struct head *find(int size){
  struct head *next = flist;
  while(next != NULL){
    if(next->size >= size){
      detach(next);
      if(next->size >= LIMIT(size)){//(HEAD + MIN(0) + size)){ //if able to split
        struct head *block = split(next, size);
        struct head *bef = before(block);
        insert_order(bef);//insert(bef);
        struct head *aft = after(block);
        aft->bfree = FALSE;
        block->free = FALSE;
        return block;
      } else { // else just give the whole block, without splitting
        next->free = FALSE;
        struct head *aft = after(next);
        aft->bfree = FALSE;
        return next;
      }
    } else {
      next = next->next;
    }
  }
  return NULL; //no block found
}

/**
* Called when a block is merged in flist to set it in its right position,
* i.e float up probably.
*/
void float_up(struct head *block){
  struct head *next = block->next;
  if(next == NULL){
    return;
  }
  if(block->size <= next->size){ //remove?
    return;
  }

  /*float is needed*/

  detach(block);

  struct head *nextPrev = next->prev;
  while((next != NULL)){
    if(block->size <= next->size){
      if(nextPrev != NULL){
        nextPrev->next = block;
      }
      next->prev = block;
      block->next = next;
      block->prev = nextPrev;
      if(next == flist){ //not needed maybe?
        flist = block;
      }
      return;
    }
    nextPrev = next;
    next = next->next;
  }
  block->next = NULL;
  nextPrev->next = block;
  block->prev = nextPrev;
}

/**
* Merge with detach and then insert (first task).
*/
struct head *merge(struct head *block){
  struct head *aft = after(block);
  if(block->bfree){
    struct head *bef = before(block);
    detach(bef);
    bef->size = bef->size + block->size + HEAD;
    aft->bsize = bef->size;
    block = bef;
  }

  if(aft->free){
    detach(aft);
    block->size = block->size + aft->size + HEAD;
    aft = after(block);
    aft->bsize = block->size;
  }

  return block;
}

/**
* Merges the block if possible without detach & insert
* to flist. It can also float the merged block to keep the flist
* ordered.
*/
struct head *merge_no_detach(struct head *block){
  struct head *aft = after(block);

  if(block->bfree){
    struct head *bef = before(block);
    bef->size = bef->size + block->size + HEAD;
    aft->bsize = bef->size;
    aft->bfree = bef->free;
    block = bef;

    if(aft->free){
      block->size = block->size + aft->size + HEAD;
      detach(aft);
      if(flist == NULL){
        flist = block;
      }
      aft = after(aft);
      aft->bsize = block->size;
      aft->bfree = block->free;
    }
    float_up(block); // keep flist ordered
    return NULL;
  }
  //only block after is free
  if(aft->free){
    block->next = aft->next;
    block->prev = aft->prev;
    struct head *aftprev = aft->prev;
    struct head *aftnext = aft->next;
    if(aftprev != NULL){
      aftprev->next = block;
    }
    if(aftnext != NULL){
      aftnext->prev = block;
    }
    if(aft == flist){
      flist = block;
    }
    block->size = block->size + aft->size + HEAD;
    block->free = TRUE;
    aft = after(block);
    aft->bsize = block->size;
    aft->bfree = block->free;
    float_up(block); // keep flist ordered
    return NULL;
  }

  return block;
}

/**
* Tries to palloc memory of the given size.
*/
void *palloc(size_t request){
  if(request <= 0){
    return NULL;
  }
  int size = adjust(request);
  struct head *taken = find(size);
  if(taken == NULL){
    return NULL;
  } else {
    return HIDE(taken);
  }
}

/**
* Prees the given memory block.
*/
void pree(void *memory){
  if(memory != NULL){
    struct head *block = MAGIC(memory);
    block = merge_no_detach(block);//merge(block);

    // Uncomment this and comment rest below to
    // just only insert...
    /*
    struct head *aft = after(block);
    block->free = TRUE;
    aft->bfree = TRUE;
    insert(block);
    */

    //maybe add sink function when splitting? ;)

    // Uncomment this and comment rest below to
    // merge without "detach" (insert rly)

    if(block == NULL){
      //merged into flist already, no need to do more...
    } else {
      block->free = TRUE;
      struct head *aft = after(block);
      aft->bfree = TRUE;
      insert_order(block);//insert(block);
    }


    // Uncomment this for merge with always detach & insert
    /*
    struct head *aft = after(block);
    block->free = TRUE;
    aft->bfree = TRUE;
    insert(block);
    */
  }
}

/**
* Check that everything is OK
*/
void sanity(){
  struct head *next = flist;
  struct head *prev = next->prev;
  uint16_t sizeOfFlist = 0;
  uint16_t sizeOfFreeArena = 0;

  printf("Checking flist...\n");
  while(next != NULL){
    if(next->free != TRUE){
      printf("There's a chunk in flist that is not free!\n");
      printf("Terminating sanity...\n");
      exit(1);
    }
    int size = next->size;
    /* add size of flist for later */
    sizeOfFlist += size;

    if(size < MIN(0)){
      printf("There's a chunk in flist that is less than MIN bytes in size!\n");
      printf("Terminating sanity...\n");
      exit(1);
    }
    if((size % ALIGN) != 0){
      printf("There's a chunk in flist that is not a multiple of 8!\n");
      printf("Terminating sanity...\n");
      exit(1);
    }
    if(next->prev != prev){
      printf("There's a chunk in flist that has wrong previous!\n");
      printf("Terminating sanity...\n");
      exit(1);
    }
    //printf("%d\t%d\n", next->size, next->free);
    prev = next;
    next = next->next;
  }

  // Comment this if order does not matter
  printf("Checking flist order...\n");
  next = flist;
  prev = next;
  while(next != NULL){
    if(prev->size > next->size){
      printf("Wrong order in flist: prev is bigger than next!\n");
      printf("prev: %d\tnext: %d\n", prev->size, next->size);
      printf("Terminating sanity\n");
      exit(1);
    }
    prev = next;
    next = next->next;
  }
  printf("flist order OK!\n");
  printf("flist OK!\n");

  unsigned int tot = ARENA;
  unsigned int count = 0;
  next = arena;
  uint16_t bf = next->bfree;
  uint16_t bs = next->bsize;
  struct head *aft = after(next);
  printf("Checking arena...\n");
  while((next->size != 0) || (next->free != FALSE)){
    if(bf != next->bfree){
      printf("Field bfree does not match the previous block's bfree\n");
      printf("Terminating sanity...\n");
      exit(1);
    }

    if(bs != next->bsize){
      printf("Field bsize does not match the previous block's bsize\n");
      printf("Terminating sanity...\n");
      exit(1);
    }

    if((next->free == TRUE) && (aft->free == TRUE)){
      printf("Two consecutive free blocks found in arena\n");
      printf("%d\t%d\n", next->free, aft->free);
      printf("Terminating sanity...\n");
      exit(1);
    }
    /* add size if free block for later */
    if(next->free == TRUE){
      sizeOfFreeArena += next->size;
    }
    /* add size to counter */
    count += HEAD + next->size;
    //printf("%d\t%d\n", next->size, next->free);
    bf = next->free;
    bs = next->size;
    next = after(next);
    aft = after(next);
  }
  /* calculate arena size by adding sentinel size to count */
  if((count + HEAD) != tot){
    printf("Arena size is not the same!\n");
    printf("Terminating sanity...\n");
    exit(1);
  }
  printf("arena OK!\n");

  printf("Comparing size of free blocks in arena with size of flist...\n");
  if(sizeOfFlist != sizeOfFreeArena){
    printf("Size not the same!\n");
    printf("Terminating sanity...\n");
    exit(1);
  }
  printf("Size match OK!\n");
  printf("Sanity passed!\n");
}

void printCountLengthOfFlist(int numOfAllocs){
  int count = 0;
  struct head *next = flist;
  while(next != NULL){
    count++;
    next = next->next;
  }
  printf("%d\t%d\n", numOfAllocs, count);
}

void printSizeDistributionOfFlist(int buffSize){
  struct head *next = flist;
  int count = 0;
  while(next != NULL){
    printf("%d\t%d\n", count, next->size);
    next = next->next;
    count++;
  }
}

void printAverageSizeDistributionOfFlist(){
  struct head *next = flist;
  int sum = 0;
  int length = 0;
  while(next != NULL){
    length++;
    sum+=next->size;
    next = next->next;
  }

  double average = (double) sum / length;
  printf("%f\t%d\n", average, length);
}


void init(){
  struct head *first = new();
  insert(first);
  // printf("%d\n", flist->size);
}

void terminate(){
  munmap(arena, ARENA);
  arena = NULL;
  flist = NULL;
}
