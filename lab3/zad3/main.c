#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/stat.h>
#include <dirent.h>

#include <errno.h>

long getFileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);
    return sz;
}

int containsString(char* filePath, char* string) {
    FILE* fptr = fopen(filePath, "r");
    if (fptr == NULL) {
        return 0;
    }

    long filesize = getFileSize(fptr);
    char* buff = calloc(filesize + 1, sizeof(char));

    fread(buff, sizeof(char), filesize, fptr);
    fclose(fptr);

    int res = strstr(buff, string) != NULL;

    free(buff);

    return res;
}

void searchDir(char* rootPath, char* pattern, int depth) {
    char* usedRootPath = calloc(256 + 1, sizeof(char));
    strcpy(usedRootPath, rootPath);
    DIR* dir = opendir(usedRootPath);

    char* newPath = calloc(256 + 1, sizeof(char));
    struct dirent* file;
    while ((file = readdir(dir)) != NULL && depth >= 0) {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            continue;
        }

        strcpy(newPath, usedRootPath);
        if (newPath[strlen(newPath) - 1] != '/') {
            strcat(newPath, "/");
        }
        strcat(newPath, file->d_name);

        struct stat sb;
        if (lstat(newPath, &sb) < 0) {
            fprintf(stderr, "unable to lstat file %s: %s\n", newPath, strerror(errno));
            free(usedRootPath);
            free(newPath);
            closedir(dir);
            exit(-1);
        }

        if (S_ISREG(sb.st_mode)) {
            if (containsString(newPath, pattern)) {
                printf("%d\t\t%s\n", getpid(), newPath);
            }
        }
        if (S_ISDIR(sb.st_mode)) {
            if (fork() == 0) {
                closedir(dir);
                --depth;
                dir = opendir(newPath);
                strcpy(usedRootPath, newPath);
                free(newPath);
                newPath = calloc(256 + 1, sizeof(char));
            }
        }
    }
    free(usedRootPath);
    free(newPath);
    closedir(dir);
}



int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "<root dir> <text pattern> <max depth>\n");
        exit(-1);
    }
    char* rootDir = argv[1];
    char* pattern = argv[2];
    int maxDepth = atoi(argv[3]);

    searchDir(rootDir, pattern, maxDepth);

    while(wait(NULL) > 0);

}
