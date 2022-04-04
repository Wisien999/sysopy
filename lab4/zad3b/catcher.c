#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

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
int senderPID = 0;

void counterHandler(int sig, siginfo_t* info, void* context) {
    if (sig != COUNT_SIGNAL) { // should not happen - just in case
        printf("not COUNT\n");
        return;
    }
    ++receivedSignals;
    senderPID = info->si_pid;

    if (mode == KILL || mode == SIGRT) {
        kill(senderPID, COUNT_SIGNAL);
    }
    else {
        union sigval value;
        value.sival_int = receivedSignals;
        sigqueue(senderPID, COUNT_SIGNAL, value);
    }
}

void endSignalHandler(int sig, siginfo_t* info, void* context) {
    if (sig != END_SIGNAL) { // should not happen - just in case
        printf("not END\n");
        return;
    }
    senderPID = info->si_pid;

    //print no of received signals
    printf("no of catched signals: %d\n", receivedSignals);
    exit(0);
}

void setupSignal(int sig, void (*f)(int, siginfo_t*, void*)) {
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = f;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, sig);
    
    if (sigaction(sig, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGUSR1");
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

int main(int argc, char **argv)
{
    if (argc <= 1) {
        fprintf(stderr, "<mode: kill|sigqueue|sigrt>");
        return 1;
    }
    setMode(argv[1]);
    printf("Catcher PID: %d\n", getpid());

    //block other signals
    sigset_t blockMask;
    sigfillset(&blockMask);
    sigdelset(&blockMask, SIGINT);
    sigdelset(&blockMask, COUNT_SIGNAL);
    sigdelset(&blockMask, END_SIGNAL);
    sigprocmask(SIG_BLOCK, &blockMask, NULL);

    // setup handlers
    setupSignal(COUNT_SIGNAL, counterHandler);
    setupSignal(END_SIGNAL, endSignalHandler);

    // setup thread suspention
    
//    sigset_t mask;
//    sigfillset(&mask);
//    //sigdelset(&mask, COUNT_SIGNAL);
//    sigdelset(&mask, END_SIGNAL);
//    sigsuspend(&mask);

    while (1) {
        usleep(10);
    }


    return 0;
}
