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
    struct threadNode *next;
};
typedef struct threadNode threadNode;

struct threadQueue {
    threadNode *first;
};
typedef struct threadQueue threadQueue;

struct sem_t {
    int count;
    threadNode *q;
};
typedef struct sem_t sem_t;

struct messageNode {
    char *message;     // copy of the message
    int len;          // length of the message
    int sender;       // TID of sender thread
    int receiver;     // TID of receiver thread
    struct messageNode *next; // pointer to next node
};

typedef struct {
    struct messageNode *msg;       // message queue
    sem_t *mbox_sem;
} mbox;

#endif
