#include "t_lib.h"

tcb *running;
tcb *ready;


/**
 *  Initialize the thread library by setting up the "running" and the "ready" queues,
 *  creating TCB of the "main" thread, and inserting it into the running queue.
 */
void t_init() {
    //printf("t_init\n");

    tcb *tmp_block;
    tmp_block = malloc(sizeof(tcb));
    tmp_block->thread_id = 1;
    tmp_block->thread_priority = 1;
    tmp_block->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    tmp_block->next = NULL;

    getcontext(tmp_block->thread_context);    /* let tmp be the context of main() */

    running = tmp_block;

//    tcb *readyBlock;
//    readyBlock = malloc(sizeof(tcb));
//    readyBlock->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));

//    ready = readyBlock;
}

/**
 *  Shut down the thread library by freeing all the dynamically allocated memory.
 */
void t_shutdown() {
    //printf("t_shutdown\n");
    while (NULL != ready->next) {
        tcb *temp_block = ready;
        ready = ready->next;
        free(temp_block->thread_context);
        free(temp_block);
    }
    free(ready->thread_context);
    free(ready);
    free(running);
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
    //printf("t_create\n");
    //printf("Creating new thread %d\n", id);
    size_t sz = 0x10000;
    tcb *new_thread_block = malloc(sizeof(tcb));
    new_thread_block->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));

    getcontext(new_thread_block->thread_context);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
    new_thread_block->thread_context->uc_stack.ss_sp = malloc(sz);  /* new statement */
    new_thread_block->thread_context->uc_stack.ss_size = sz;
    new_thread_block->thread_context->uc_stack.ss_flags = 0;
    makecontext(new_thread_block->thread_context, (void (*)(void)) fct, 1, id);

    new_thread_block->thread_id = id;
    new_thread_block->thread_priority = pri;
    new_thread_block->thread_context = new_thread_block->thread_context;
    new_thread_block->next = NULL;

    tcb *parent_control_block = ready;
    if (NULL == parent_control_block) {
        //printf("ready null, assigning new thread to ready\n");
        ready = new_thread_block;
    } else {
        //printf("ready not null, parsing list to find tail\n");
        while (NULL != parent_control_block->next) {
            parent_control_block = parent_control_block->next;
        }
        parent_control_block->next = new_thread_block;
    }
    return 0;
}

/**
 * Terminate the calling thread by removing (and freeing) its TCB from the "running" queue,
 * and resuming execution of the thread in the head of the "ready" queue via `setcontext()`.
 */
void t_terminate() {
    //printf("t_terminate\n");
    tcb *tmp = running;
    running = ready;
    ready = ready->next;
    free(tmp->thread_context->uc_stack.ss_sp);
    free(tmp->thread_context);
    free(tmp);

    if (NULL != running) {
        setcontext(running->thread_context);
    }
}

/**
 * The calling thread voluntarily relinquishes the CPU,
 * and is placed at the end of the ready queue.
 * The first thread (if there is one) in the ready queue resumes execution.
 */
void t_yield() {
    //printf("t_yield\n");
    tcb *current_ready_queue = ready;
    tcb *current_running_queue = running;
    running->next = NULL;

    // Add current running process to the end of the ready queue
    if (NULL != current_ready_queue) {
        while (NULL != current_ready_queue->next) {
            current_ready_queue = current_ready_queue->next;
        }
        current_ready_queue->next = current_running_queue;
        tcb *last = current_running_queue;
        running = ready;
        ready = ready->next;

        swapcontext(last->thread_context, running->thread_context);
    }
}