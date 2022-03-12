#ifndef ZAD1_LIBRARY_H
#define ZAD1_LIBRARY_H

void libzad1_createMemory(int size);
void libzad1_removeMemory();
void libzad1_wcFiles(char** files, int n);
int libzad1_loadFile();
void libzad1_removeBlock(int index);
char* getBlock(int index);

#endif //ZAD1_LIBRARY_H
