/*
 * Test Program #8 - Asynchronous Send/Receive
 */

#include <stdio.h>
#include <stdlib.h>
#include "src/ud_thread.h"

void producer(int val) {
    char msg[] = "hello world...";
    int i = 0;

    for (i = 0; i < 4; i++) {
        msg[12] = (char) i + 0x30;
        send(2, msg, strlen(msg));
        printf("thread %d [%s]...\n", val, msg);
    }

    printf("thread %d terminates...\n", val);
    t_terminate();
}

void consumer(int val) {
    int len, who = 0;
    char *msg;

    msg = (char *) malloc(1024);

    do {
        who = 0;
        receive(&who, msg, &len);
        if (who != 0)
            printf("I got message [%s] from %d...\n", msg, who);
    } while (who != 0);

    t_terminate();
}

int main(void) {
    int i;

    t_init();

    t_create(producer, 1, 1);
    t_create(producer, 3, 1);
    t_create(consumer, 2, 1);

    t_yield();

    t_shutdown();

    return 0;
}


/*
thread 1 [hello world.0.]...
thread 1 [hello world.1.]...
thread 1 [hello world.2.]...
thread 1 [hello world.3.]...
thread 1 terminates...
thread 3 [hello world.0.]...
thread 3 [hello world.1.]...
thread 3 [hello world.2.]...
thread 3 [hello world.3.]...
thread 3 terminates...
I got message [hello world.0.] from 1...
I got message [hello world.1.] from 1...
I got message [hello world.2.] from 1...
I got message [hello world.3.] from 1...
I got message [hello world.0.] from 3...
I got message [hello world.1.] from 3...
I got message [hello world.2.] from 3...
I got message [hello world.3.] from 3...
 */