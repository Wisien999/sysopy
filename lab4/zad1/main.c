#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("%d recived\n", sig);
}

int main(int argc, char** argv) {
    if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0)
    {
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &newmask, NULL) < 0)
            perror("Nie udało się zablokować sygnału");
    }

    else if (strcmp(argv[1], "handler") == 0)
    {
        signal(SIGUSR1, handler);
    }
    else if (strcmp(argv[1], "ignore") == 0)
    {
        signal(SIGUSR1, SIG_IGN);
    }

    raise(SIGUSR1);
    sigset_t newmask;

    if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0)
    {
        sigpending(&newmask);
        printf("Signal pending: %d\n", sigismember(&newmask, SIGUSR1));
    }

    if (argc >= 3 && strcmp(argv[2], "exec") == 0)
    {
        execl("./exec", "./exec", argv[1], NULL);
    }
    else {
        int cpid = fork();
        if (cpid == 0) {
            if (strcmp(argv[1], "pending") != 0) {
                raise(SIGUSR1);
            }

            if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0)
            {
                sigset_t childmask;
                sigpending(&childmask);
                printf("Child signal pending: %d\n", sigismember(&childmask, SIGUSR1));
            }
        }
    }

    return 0;
}
