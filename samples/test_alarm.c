/*
 * signal() and SIGALRM
 * sighold(): defer signal
 * sigrelse(): enable signal
 *
 * pause() is used to "simulate" that this process
 * is "busy" so that no "busy computation" is used.
 *
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void sig_func(int sig_no) {
    printf("*** signal [%d](%s) is caught...\n", sig_no,
           strsignal(sig_no));
}

int main() {
    printf("Starting...\n");
    int ret;

    signal(SIGALRM, sig_func);

    //ualarm(900000, 0);  // alarm in 3 seconds
    alarm(3);               // alarm in 3 seconds

    // version 1: deferred SIGALRM signal
//     sighold(SIGALRM);
//     usleep(6000000);     // sleep 6 seconds
//     sigrelse(SIGALRM);
    // end of version 1

    // version 2: normal SIGALRM signal
    ret = pause();
    usleep(6000000);  // sleep 6 seconds
    printf("pause() returns: (%d) [%s]\n", ret, strerror(errno));
    // end of version 2

    return printf("The end...\n");
}
