#define _GNU_SOURCE /* to define REG_RIP */

#include <stdio.h>
#include <signal.h>
#include <ucontext.h>

static void handler(int sig_no, siginfo_t *info, void *cntx){
  ucontext_t *context = (ucontext_t*)cntx;
  unsigned long pc = context->uc_mcontext.gregs[REG_RIP];

  printf("Illegal instruction at %lx value 0x%x\n", pc, *(int *)pc);
  context->uc_mcontext.gregs[REG_RIP] = pc + 1;
}

int main(){
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = handler;

  sigemptyset(&sa.sa_mask);

  sigaction(SIGSEGV, &sa, NULL);

  printf("Let's go!\n");

  /*Don't do this...*/
  asm(".word 0x00000000");

  here:
    printf("Piece of cake, this call is here 0x%x!\n", &&here);
    return 0;
}