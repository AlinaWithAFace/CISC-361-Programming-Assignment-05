#include "t_lib.h"
#include <signal.h>
#include <unistd.h>
#include "ud_thread.h"

threadNode *running;
threadQueue *ready;


/**
 *  Initialize the thread library by setting up the "running" and the "ready" queues,
 *  creating TCB of the "main" thread, and inserting it into the running queue.
 */
void t_init() {
    init_alarm();

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
}


/**
 * Initialize alarm system and timeout
 */
void init_alarm() {
    signal(SIGALRM, sig_func);
    ualarm(10000, 0); // alarm in 1 microsecond
}


/**
 *  Shut down the thread library by freeing all the dynamically allocated memory.
 */
void t_shutdown() {
    printf("t_shutdown\n");

    threadNode *current = ready->first;
    while (NULL != current->next) {
        threadNode *temp_block = current;
        current = current->next;
        free(temp_block->thread_context);
        free(temp_block);
    }
    free(current->thread_context);
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
void t_create(void (*fct)(int), int id, int pri) {
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

    push(ready, new_thread_block);
}

/**
 * Terminate the calling thread by removing (and freeing) its TCB from the "running" queue,
 * and resuming execution of the thread in the head of the "ready" queue via `setcontext()`.
 */
void t_terminate() {
    threadNode *tmp = running;
    free(tmp->thread_context->uc_stack.ss_sp);
    free(tmp->thread_context);
    free(tmp);

    running = pop(ready);
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
    threadNode *next = pop(ready);
    threadNode *current = running;
    next->next = NULL;
    current->next = NULL;

    push(ready, current);
    running = next;

    init_alarm();
    swapcontext(current->thread_context, next->thread_context);
}

void sig_func(int sig_no) {
    t_yield();
}

/**
 * Add a node to the given queue
 * @param node
 */
void push(threadQueue *queueHead, threadNode *node) {
    node->next = NULL;
    threadNode *currentNode = queueHead->first;

    /// Insert node at the beginning of the queue if it's empty
    if (NULL == currentNode) {
        queueHead->first = node;
    } else {
        /// New node's priority is lesser than first item in queue
        if (node->thread_priority < queueHead->first->thread_priority) {
            node->next = queueHead->first;
            queueHead->first = node;
        } else {
            /// Traverse to the end of the list or to the point the priority is lesser
            while (NULL != currentNode->next && currentNode->next->thread_priority < node->thread_priority) {
                currentNode = currentNode->next;
            }
            currentNode->next = node;
        }
    }
}


threadNode *pop(threadQueue *queueHead) {
    threadNode *tmp = NULL;
    if (NULL != queueHead) {
        tmp = queueHead->first;
        queueHead->first = queueHead->first->next;
        tmp->next = NULL;
    }
    if (NULL != tmp) {
        tmp->next = NULL;
    }
    return tmp;
}

void printList(threadQueue *queueHead) {
    threadNode *currentNode;
    printf("[%d] ", running->thread_id);
    if (NULL != queueHead) {
        printf("{");
        currentNode = queueHead->first;

        while (NULL != currentNode) {
            printf("%d, ", currentNode->thread_id);
            currentNode = currentNode->next;
        }
        printf("}");
    } else {
        printf("{ }");
    }
    printf("\n");
}

int sem_init(sem_t **sp, int sem_count) {
  /* TO DO: Create new semaphore pointed to by sp with count val sem_count
            Return int upon success/failure? Maybe*/
  sem_t *tmp = (sem_t *) malloc(sizeof(sem_t));
  tmp->count = sem_count;
  tmp->q = NULL;
  sp = tmp;
  return 1; //upon successful completion? eh
}

void sem_wait(sem_t *sp) {
  /*TO DO: Current thread does wait (P) on semaphore */
  sighold();
  sigrelse();
}

void sem_signal(sem_t *sp) {
  /*TO DO: Current thread does signal (V) on specified semaphore. Follow Mesa semantics*/
  sighold();
  
  sigrelse();
}

void sem_destroy(sem_t **sp) {
  sighold();

  sem_t *tmp = *sp;
  threadNode *next;

  while(tmp->q != NULL) {
    next = tmp->q->next;
    free(tmp->q->thread_context.uc_stack.ss_sp);
    free(tmp->q->thread_context);
    free(tmp);
    tmp = next->q;
  }
  free(tmp);
  
  sigrelse();
}
