#ifndef WC_DYN_H
#define WC_DYN_H
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static void *handle = NULL;

void (*_libzad1_createMemory)(int size);
void (*_libzad1_removeMemory)();
void (*_libzad1_wcFiles)(char** files, int n);
int (*_libzad1_loadFile)();
void (*_libzad1_removeBlock)(int index);

void initDynLib() {
#ifdef DYN_WC_LIB_NAME
    printf("dyn name: %s\n", DYN_WC_LIB_NAME);
    handle = dlopen(DYN_WC_LIB_NAME, RTLD_NOW);
#endif
    if (handle == NULL) {
        fprintf(stderr, "Error dynamic library open\n");
        return;
    }

    _libzad1_createMemory = dlsym(handle, "libzad1_createMemory");
    _libzad1_removeMemory = dlsym(handle, "libzad1_removeMemory");
    _libzad1_wcFiles = dlsym(handle, "libzad1_wcFiles");
    _libzad1_loadFile = dlsym(handle, "libzad1_loadFile");
    _libzad1_removeBlock = dlsym(handle, "libzad1_removeBlock");

    char *error;
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

void libzad1_createMemory(int size) {
    return (*_libzad1_createMemory)(size);
}
void libzad1_removeMemory() {
    return (*_libzad1_removeMemory)();
}
void libzad1_wcFiles(char** files, int n) {
    return (*_libzad1_wcFiles)(files, n);
}
int libzad1_loadFile() {
    return (*_libzad1_loadFile)();
}
void libzad1_removeBlock(int index) {
    return (*_libzad1_removeBlock)(index);
}


#endif //ZAD1_LIBRARY_H
