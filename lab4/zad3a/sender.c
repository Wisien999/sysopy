#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>


typedef enum Mode
{
    KILL,
    QUEUE,
    SIGRT
} Mode;

int COUNT_SIGNAL;
int END_SIGNAL;
Mode mode;
int receivedSignals = 0;
int noOfSignalsToSend;

void handle_signals(int sig, siginfo_t *info, void *ucontext)
{
    if (sig == COUNT_SIGNAL)
    {
        receivedSignals++;
        if (mode == QUEUE)
        {
            printf("already received: %d, catcher index: %d\n", receivedSignals, info->si_value.sival_int);
        }
    }
    else if (sig == END_SIGNAL)
    {
        printf("sender received: %d signals, should receive: %d\n", receivedSignals, noOfSignalsToSend);
        exit(0);
    }
}

void setMode(char* modeStr) {
    if (strcmp("kill", modeStr) == 0)
    {
        mode = KILL;
        COUNT_SIGNAL = SIGUSR1;
        END_SIGNAL = SIGUSR2;
    }
    else if (strcmp("sigqueue", modeStr) == 0)
    {
        mode = QUEUE;
        COUNT_SIGNAL = SIGUSR1;
        END_SIGNAL = SIGUSR2;
    }
    else if (strcmp("sigrt", modeStr) == 0)
    {
        mode = SIGRT;
        COUNT_SIGNAL = SIGRTMIN + 1;
        END_SIGNAL = SIGRTMIN + 2;
    }
    else
    {
        fprintf(stderr, "wrong mode, allowed: [kill, sigqueue, sigrt]\n");
        exit(1);
    }
}


int main(int argc, char** argv) {
    if (argc <= 3) {
        printf("<no of signals> <catcher PID> <mode: kill|sigqueue|sigrt>");
        return 0;
    }
    noOfSignalsToSend = atoi(argv[1]);
    int catcherPID = atoi(argv[2]);

    char* mode_str = argv[3];
    setMode(mode_str);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigdelset(&mask, COUNT_SIGNAL);
    sigdelset(&mask, END_SIGNAL);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    {
        perror("cant block signals\n");
        exit(1);
    }


    struct sigaction sa_handle;
    sa_handle.sa_flags = SA_SIGINFO;
    sa_handle.sa_sigaction = handle_signals;

    sigemptyset(&sa_handle.sa_mask);
    sigaddset(&sa_handle.sa_mask, COUNT_SIGNAL);
    sigaddset(&sa_handle.sa_mask, END_SIGNAL);

    sigaction(COUNT_SIGNAL, &sa_handle, NULL);
    sigaction(END_SIGNAL, &sa_handle, NULL);

    printf("created sender with PID: %d\n", getpid());


    if (mode == KILL || mode == SIGRT) {
        printf("kill or sigrt %d \n", noOfSignalsToSend);
        for (int i = 0; i < noOfSignalsToSend; ++i) {
            kill(catcherPID, COUNT_SIGNAL);
        }
        kill(catcherPID, END_SIGNAL);
    }
    else {
        union sigval val;
        val.sival_int = 0;
        for (int i = 0; i < noOfSignalsToSend; ++i) {
            sigqueue(catcherPID, COUNT_SIGNAL, val);
        }
        sigqueue(catcherPID, END_SIGNAL, val);
    }


    while (1) {
        usleep(10);
    }


    return 0;
}
