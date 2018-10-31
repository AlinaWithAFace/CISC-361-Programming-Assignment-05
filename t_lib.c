#include "t_lib.h"

tcb *running;
tcb *ready;


/**
 *  Initialize the thread library by setting up the "running" and the "ready" queues,
 *  creating TCB of the "main" thread, and inserting it into the running queue.
 */
void t_init() {

    tcb *tmp_block;
    tmp_block = malloc(sizeof(tcb));
    tmp_block->thread_id = 0;
    tmp_block->thread_priority = 0;
    tmp_block->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    tmp_block->next = NULL;

    getcontext(tmp_block->thread_context);    /* let tmp be the context of main() */
    running = tmp_block;

    ready = tmp_block;
}

/**
 *  Shut down the thread library by freeing all the dynamically allocated memory.
 */
void t_shutdown() {
    free(running);
    while (NULL != ready->next) {
        tcb *temp_block = ready;
        ready = ready->next;
        free(temp_block->thread_context);
        free(temp_block);
    }
    free(ready->thread_context);
    free(ready);
}

/**
 * Create a thread with priority `pri`,
 * start function `func` with argument `thr_id` as the thread id.
 *
 * Function `func` should be of type void func(int).
 *
 * TCB of the newly created thread is added to the *end* of the "ready" queue;
 * the parent thread calling `t_create()` continues its execution upon returning from `t_create()`.
 * @param fct function
 * @param id thread id
 * @param pri priority
 * @return
 */
int t_create(void (*fct)(int), int id, int pri) {
    size_t sz = 0x10000;

    ucontext_t *new_context;
    new_context = (ucontext_t *) malloc(sizeof(ucontext_t));

    getcontext(new_context);

    new_context->uc_stack.ss_sp = mmap(0, sz,
                                       PROT_READ | PROT_WRITE | PROT_EXEC,
                                       MAP_PRIVATE | MAP_ANON, -1, 0);

    new_context->uc_stack.ss_sp = malloc(sz);  /* new statement */
    new_context->uc_stack.ss_size = sz;
    new_context->uc_stack.ss_flags = 0;
    new_context->uc_link = running->thread_context;
    makecontext(new_context, (void (*)(void)) fct, 1, id);

    //ready = new_context;

    tcb *new_control_block;
    new_control_block = malloc(sizeof(tcb));
    new_control_block->thread_id = id;
    new_control_block->thread_priority = pri;
    new_control_block->thread_context = new_context;
    new_control_block->next = NULL;

    tcb *parent_control_block;
    parent_control_block = ready;
    while (NULL != parent_control_block->next) {
        parent_control_block = parent_control_block->next;
    }
    parent_control_block->next = new_control_block;
}

/**
 * Terminate the calling thread by removing (and freeing) its TCB from the "running" queue,
 * and resuming execution of the thread in the head of the "ready" queue via `setcontext()`.
 */
void t_terminate() {
    //todo
}

/**
 * The calling thread voluntarily relinquishes the CPU,
 * and is placed at the end of the ready queue.
 * The first thread (if there is one) in the ready queue resumes execution.
 */
void t_yield() {
    tcb *current_thread;

    current_thread = running;

    tcb *tmp_control_block;
    tmp_control_block = ready;

    // Add current running process to the end of the ready queue
    while (NULL != tmp_control_block->next) {
        tmp_control_block = tmp_control_block->next;
    }
    tmp_control_block->next = current_thread;

    swapcontext(ready->thread_context, running->thread_context);
    ready = ready->next;
}