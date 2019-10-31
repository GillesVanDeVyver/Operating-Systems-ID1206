#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global [] = "This is a global string";
const int read_only = 123456;


int main() {
  int pid = getpid();
  
  unsigned long p = 0x1;

  printf(" p (0x%lx): %p \n", p, &p); 

    printf("\n\n /proc/%d/maps \n\n", pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
}