#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, FALSE};

static green_t *running = &main_green;

static void init() __attribute__((constructor));

void init()
{
    getcontext(&main_cntx);
}

int green_create(green_t *new, void *(*fun)(void *), void *arg)
{

    ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void *stack = malloc(STACK_SIZE);

    cntx->uc_stack.ss_sp = stack;
    cntx->uc_stack.ss_size = STACK_SIZE;
    makecontext(cntx, green_thread, 0);

    new->context = cntx;
    new->fun = fun;
    new->arg = NULL;
    new->next = NULL;
    new->join = NULL;
    new->zombie = FALSE;

    add_to_ready_queue(new);
}

void add_to_ready_queue(green_t *ready)
{
    add_to_queue(&running, ready);
}

void add_to_queue(green_t **queue, green_t *thread_to_add)
{
    green_t *current = queue;
    if (current == NULL)
    {
        *queue = thread_to_add;
    }
    else
    {
        while (current->next != NULL)
            current = current->next;

        current = thread_to_add;
    }
}

void green_thread()
{

    sigprocmask(SIG_UNBLOCK, &block, NULL);

    green_t *this = running;

    (*this->fun)(this->arg);

    sigprocmask(SIG_BLOCK, &block, NULL);

    //add joining thread to ready queue

    if (this->join != NULL)
        add_to_ready_queue(this->join);

    //free allocated memory structures

    free(this->context->uc_stack.ss_sp);
    free(this->context);

    //we're a zombie
    this->zombie = TRUE;

    //find the next thread to run
    set_next_running();

    setcontext(running->context); //Threads life ends here
}

void set_next_running()
{

    pop_from_queue(&running);
    if (running == NULL)
        printf("Deadlock, no thread ready to run!\n");
}

green_t *pop_from_queue(green_t **queue)
{
    green_t *popped = *queue;

    if (popped != NULL)
    {
        *queue = popped->next;
        popped->next = NULL;
    }
    return popped;
}

//For debugging
int queue_length(green_t *queue)
{

    green_t *current = queue;
    int counter = 1;
    while (current->next != NULL)
    {
        current = current->next;
        counter++;
    }
    return counter;
}