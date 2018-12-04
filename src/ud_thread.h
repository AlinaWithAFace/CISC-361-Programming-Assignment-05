/*
 * thread library function prototypes
 */

#include "t_lib.h"


#ifndef _UDTHLB_
#define _UDTHLB_


void t_create(void(*function)(int), int thread_id, int priority);

void t_yield(void);

void t_init(void);

void t_shutdown(void);

void t_terminate(void);

void sig_func(int sig_no);

void init_alarm();

void push(threadQueue *queueHead, threadNode *node);

threadNode *pop(threadQueue *queueHead);

void printList(threadQueue *queueHead);

int mbox_create(mbox **mb);

void mbox_destroy(mbox **mb);

void mbox_deposit(mbox *mb, char *msg, int len);

void mbox_withdraw(mbox *mb, char *msg, int *len);

void send(int tid, char *msg, int len);

void receive(int *tid, char *msg, int *len);

mbox *fetchMailbox(int tid);

#endif