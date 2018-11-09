#include <memory.h>
#include "t_lib.h"
#include <signal.h>
#include <zconf.h>
#include "ud_thread.h"

threadNode *running;
threadQueue *ready;


/**
 *  Initialize the thread library by setting up the "running" and the "ready" queues,
 *  creating TCB of the "main" thread, and inserting it into the running queue.
 */
void t_init() {
    //printf("t_init\n");

    threadNode *tmp_block;
    tmp_block = malloc(sizeof(threadNode));
    tmp_block->thread_id = 0;
    tmp_block->thread_priority = 1;
    tmp_block->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
    tmp_block->next = NULL;

    getcontext(tmp_block->thread_context);    /* let tmp be the context of main() */

    running = tmp_block;

    ready = (threadQueue *) calloc(1, sizeof(threadQueue));
    ready->first = NULL;

    //init_alarm();

//    threadNode *readyBlock;
//    readyBlock = malloc(sizeof(threadNode));
//    readyBlock->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));

//    readyNode = readyBlock;
}


/**
 * Initialize alarm system and timeout
 */
void init_alarm() {
    signal(SIGALRM, sig_func);
    ualarm(100000, 0); // alarm in 1 microsecond
}


/**
 *  Shut down the thread library by freeing all the dynamically allocated memory.
 */
void t_shutdown() {
    //printf("t_shutdown\n");
//    while (NULL != readyNode->next) {
//        threadNode *temp_block = readyNode;
//        readyNode = readyNode->next;
//        free(temp_block->thread_context);
//        free(temp_block);
//    }
//    free(readyNode->thread_context);
//    free(readyNode);


    for (int i = 0; i < ready->len; ++i) {
        threadNode *tmp = pop(ready);
        free(tmp->thread_context);
        free(tmp);
    }

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
void t_create(void (*fct)(int), int id, int pri) {
    //printf("t_create\n");
    //printf("Creating new thread %d\n", id);
    size_t sz = 0x10000;
    threadNode *new_thread_block = malloc(sizeof(threadNode));
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

//    threadNode *parent_control_block = readyNode;
//    if (NULL == parent_control_block) {
//        //printf("readyNode null, assigning new thread to readyNode\n");
//        readyNode = new_thread_block;
//    } else {
//        //printf("readyNode not null, parsing list to find tail\n");
//        while (NULL != parent_control_block->next) {
//            parent_control_block = parent_control_block->next;
//        }
//        parent_control_block->next = new_thread_block;
//    }

    push(new_thread_block);
}

/**
 * Terminate the calling thread by removing (and freeing) its TCB from the "running" queue,
 * and resuming execution of the thread in the head of the "ready" queue via `setcontext()`.
 */
void t_terminate() {
    //printf("t_terminate %d\n", running->thread_id);
    threadNode *tmp = running;
    free(tmp->thread_context->uc_stack.ss_sp);
    free(tmp->thread_context);
    free(tmp);

    running = pop(ready);
    if (NULL != running) {
        //printf("setting context to thread %d", running->thread_id);
        setcontext(running->thread_context);
    }
}

/**
 * The calling thread voluntarily relinquishes the CPU,
 * and is placed at the end of the ready queue.
 * The first thread (if there is one) in the ready queue resumes execution.
 */
void t_yield() {
    threadNode *next = pop(ready);
    threadNode *current = running;

    push(current);
    running = next;

    //printf("Swapping %d and %d\n", current->thread_id, next->thread_id);
    swapcontext(current->thread_context, next->thread_context);
}

void sig_func(int sig_no) {
    printf("Caught signal (%d)[%s]\n", sig_no, strsignal(sig_no));
}


void push(threadNode *node) {
    //printList(ready);
    threadNode *currentNode = ready->first;

    if (NULL == currentNode) {
        //printf("Current node null\n");
        ready->first = node;
    } else {
        while (NULL != currentNode->next) {
            currentNode = currentNode->next;
        }
        currentNode->next = node;
    }
    //printf("[Pushing on %d]\n", node->thread_id);
    //printList(ready);
}


threadNode *pop(threadQueue *heap) {
    //printList(heap);
    threadNode *tmp = NULL;
    if (NULL != heap) {
        tmp = heap->first;
        if (NULL != heap->first->next) {
            heap->first = heap->first->next;
        }
        tmp->next = NULL;
        //printf("[Popping off %d]\n", tmp->thread_id);
    } else {
        //printf("[Popping off null]\n");
    }
    //printList(heap);
    return tmp;
}

void printList(threadQueue *heap) {
    printf("{");
    threadNode *currentNode = heap->first;
    while (NULL != currentNode) {
        printf(" %d,", currentNode->thread_id);
        currentNode = currentNode->next;
    }
    printf("}\n");
}
