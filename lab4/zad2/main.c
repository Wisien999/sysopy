#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

void sigusr1_handler(int signo, siginfo_t* info, void* context) {
    if (signo != SIGUSR1) {
        return;
    }

    if (info->si_code == SI_USER) {
        printf("SIGUSR1 (%d) tried killing (via kill), PID of sender: %d, real user ID of sender: %d\n", info->si_signo, info->si_pid, info->si_uid);
    }
}

void testCase1(void) {
    printf("test 1: \n");
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = sigusr1_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGUSR1, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGUSR1");
    }

    kill(getpid(), SIGUSR1);
}

void sigchild_handler(int signo, siginfo_t* info, void* context) {
    if (signo != SIGCHLD) {
        return;
    }

    printf("SIGCHILD (%d) child's exit code: %d, PID of sender: %d, real user ID of sender: %d\n", info->si_status, info->si_signo, info->si_pid, info->si_uid);
}

void testCase2(void) {
    printf("test 2: \n");
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = sigchild_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGCHLD, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGCHLD");
    }

    if (fork() == 0) {
        exit(9);
    }

    wait(NULL);
}

void alarm_handler(int signo, siginfo_t* info, void* context) {
    if (signo != SIGALRM) {
        return;
    }

    printf("SIGALRM (%d) , PID of sender: %d, real user ID of sender: %d\n", info->si_signo, info->si_pid, info->si_uid);
}

void testCase3(void) {
    printf("test 3: \n");
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = alarm_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGALRM, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGALRM");
    }

    alarm(2);

    pause();
}

void sigchld_simple_handler(int sig) {
    if (sig != SIGCHLD) {
        return;
    }

    printf("\nSIGCHLD (%d) happened\n", sig);
}

void testCase4(void) {
    printf("test 4: \n");
    struct sigaction action;
    action.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
    //action.sa_handler = sigchld_simple_handler;
    action.sa_sigaction = sigchild_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGCHLD, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGCHLD");
    }

    int cpid = fork();
    if (cpid > 0) {
        kill(cpid, SIGSTOP);
        sleep(1);
        kill(cpid, SIGCONT);
    }
    else if (cpid == 0) {
        int i = 0;
        while (++i < 1000);
        exit(9);
    }
}

void sigusr1_simple_handler(int sig) {
    if (sig != SIGUSR1) {
        printf("not");
        return;
    }

    printf("SIGUSR1 (%d) happened\n", sig);
}

void testCase5(void) {
    printf("test 5: \n");
    struct sigaction action;
    action.sa_flags = SA_RESETHAND;
    action.sa_handler = sigusr1_simple_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGUSR1, &action, NULL) < 0) {
        fprintf(stderr, "Can't catch SIGUSR1");
    }

    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);

    //printf("is SIGUSR1 memeber: %d\n", sigismember(&action.sa_mask, SIGUSR1));
    //kill(getpid(), SIGUSR1);
    //printf("is SIGUSR1 memeber: %d\n", sigismember(&action.sa_mask, SIGUSR1));
}


int main(int argc, char** argv) {

    testCase1();
    testCase2();
    testCase3();
    testCase4();
    testCase5();

    printf("------END-------");

    return 0;
}
