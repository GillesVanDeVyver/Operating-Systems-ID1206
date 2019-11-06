#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int volatile count;
void handler (int sig){
  printf("signal %d ouch that hurt\n", sig);
  count++;
}

int main (){

  struct sigaction siga;
  int pid = getpid();
  printf("ok, let's go, kill me (%d) if you can!\n", pid);

  siga.sa_handler = handler;
  siga.sa_flags = 0;
  sigemptyset(&siga.sa_mask);
  if (sigaction(SIGINT, &siga, NULL) != 0){
    return(1);
  }

  while(count != 4){
  }
  printf("I've had enough!\n");

  return(0);
}