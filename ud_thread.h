#include "t_lib.h"

/*
 * thread library function prototypes
 */
void t_create(void(*function)(int), int thread_id, int priority);

void t_yield(void);

void t_init(void);

void t_shutdown(void);

void t_terminate(void);

void sig_func(int sig_no);

void init_alarm();

void push(threadQueue *heap, threadNode *node);

threadNode *pop(threadQueue *heap);

void printList(threadQueue *heap);