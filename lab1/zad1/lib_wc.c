#include "lib_wc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIB_MEMORY_DATATYPE char*
const char LIB_TMP_FILENAME[] = "/tmp/wctmp.txt";

LIB_MEMORY_DATATYPE* memory = NULL;
int memoryLen = 0;
int memoryMaxLen = 0;
int memoryFirstFreeIndex = 0;


char* getBlock(int index) {
    return memory[index];
}

void libzad1_createMemory(int size) {
    if (memory != NULL) {
        fprintf(stderr, "Can't create memory twice!");
        return;
    }

    memory = calloc(size, sizeof(LIB_MEMORY_DATATYPE));
    memoryMaxLen = size;
    memoryLen = 0;
}

void libzad1_removeMemory() {
    for (int i = 0; i < memoryMaxLen; ++i) {
        if (memory[i] != NULL) {
            free(memory[i]);
            memory[i] = NULL;
        }
    }

    free(memory);
    memory = NULL;
    memoryMaxLen = 0;
    memoryLen = 0;
}

void libzad1_wcFiles(char** files, int n) {
    unsigned int sumLen = 3 + 2 + strlen(LIB_TMP_FILENAME);

    for (int i = 0; i < n; ++i) {
        sumLen += strlen(files[i]);
        ++sumLen;
    }

    char* command = calloc(sumLen+1, sizeof(char));
    strcpy(command, "wc ");
    for (int i = 0; i < n; ++i) {
        strcat(command, files[i]);
        strcat(command, " ");
    }
    strcat(command, "> ");
    strcat(command, LIB_TMP_FILENAME);
    command[sumLen] = 0;

    system(command);
    free(command);
}

long get_file_size(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);
    return sz;
}

int libzad1_loadFile() {
    if (memoryLen >= memoryMaxLen) {
        fprintf(stderr, "Memory is full");
        return -1;
    }

    FILE* fptr;
    fptr = fopen(LIB_TMP_FILENAME, "r");

    if(fptr == NULL) {
        fprintf(stderr, "Can't open tmp file");
        return -1;
    }

    memory[memoryFirstFreeIndex] = calloc(get_file_size(fptr), sizeof(char));
    fscanf(fptr, "%s", memory[memoryFirstFreeIndex]);
    fclose(fptr);

    int savedAt = memoryFirstFreeIndex;
    ++memoryLen;
    // if the memory is full then do not look for free space
    if (memoryLen >= memoryMaxLen) {
        return savedAt;
    }

//  Find next free space
    do {
        memoryFirstFreeIndex = memoryFirstFreeIndex + 1 < memoryMaxLen ? memoryFirstFreeIndex + 1 : 0;
    } while (memory[memoryFirstFreeIndex] != NULL);

    return savedAt;
}

void libzad1_removeBlock(int index) {
    if (index < 0 || index >= memoryMaxLen || memory[index] == NULL) {
        return;
    }

    free(memory[index]);
    memory[index] = NULL;

//    for (int i = index + 1; i < memoryLen; ++i) {
//        memory[i-1] = memory[i];
//    }
    --memoryLen;
}
