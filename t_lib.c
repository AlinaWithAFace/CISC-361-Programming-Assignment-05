#include "t_lib.h"

ucontext_t *running;
ucontext_t *ready;

/**
 * The calling thread voluntarily relinquishes the CPU,
 * and is placed at the end of the ready queue.
 * The first thread (if there is one) in the ready queue resumes execution.
 */
void t_yield() {
    ucontext_t *tmp;

    tmp = running;
    running = ready;
    ready = tmp;

    swapcontext(ready, running);
}


/**
 *  Initialize the thread library by setting up the "running" and the "ready" queues,
 *  creating TCB of the "main" thread, and inserting it into the running queue.
 */
void t_init() {
    ucontext_t *tmp;
    tmp = (ucontext_t *) malloc(sizeof(ucontext_t));

    getcontext(tmp);    /* let tmp be the context of main() */
    running = tmp;
}

/**
 *  Shut down the thread library by freeing all the dynamically allocated memory.
 */
void t_shutdown() {
    //todo
}

/**
 * Create a thread with priority `pri`,
 * start function `func` with argument `thr_id` as the thread id.
 *
 * Function `func` should be of type void func(int).
 *
 * TCB of the newly created thread is added to the *end* of the "ready" queue;
 * the parent thread calling `t_create()` continues its execution upon returning from `t_create()`.
 * @param fct
 * @param id
 * @param pri
 * @return
 */
int t_create(void (*fct)(int), int id, int pri) {
    size_t sz = 0x10000;

    ucontext_t *uc;
    uc = (ucontext_t *) malloc(sizeof(ucontext_t));

    getcontext(uc);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
    uc->uc_stack.ss_sp = malloc(sz);  /* new statement */
    uc->uc_stack.ss_size = sz;
    uc->uc_stack.ss_flags = 0;
    uc->uc_link = running;
    makecontext(uc, (void (*)(void)) fct, 1, id);
    ready = uc;
}

/**
 * Terminate the calling thread by removing (and freeing) its TCB from the "running" queue,
 * and resuming execution of the thread in the head of the "ready" queue via `setcontext()`.
 */
void t_terminate() {
    //todo
}