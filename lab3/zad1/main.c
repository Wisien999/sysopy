#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "<no of processes>");
        exit(-1);
    }
    int n = atoi(argv[1]);
    int childPID;

    for (int i = 0; i < n; ++i) {
        childPID = fork();
        if (childPID == 0) { break; }
    }

    if (childPID == 0) {
        printf("I am a child! ");
    }
    else {
        printf("I am a parent! ");
    }

    printf("My ID: %d\n", getpid());
}
