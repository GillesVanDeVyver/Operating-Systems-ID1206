#include <stdio.h>
#include <stdlib.h>

int counter = 0;
void hello(char *name){
  printf("Hello: %s, %d\n",  name, counter);
}
int main(int argc, char **argv) {
  char *me = argv[1];
  while(counter != 100) {
     counter++;
     hello(me);
     sleep(1);
}
return 0; 
}