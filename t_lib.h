/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>


#ifndef _TLIBH_
#define _TLIBH_

struct threadNode {
    int thread_id; // level 0 (high priority) and 1 (low priority)
    int thread_priority;
    ucontext_t *thread_context;
};
typedef struct threadNode threadNode;

struct threadHeap {
    threadNode **nodes;
    int len;
    int size;
};
typedef struct threadHeap threadHeap;

#endif

