#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <time.h>
#include <sys/stat.h>

#include <dirent.h>


struct Counter {
    int files;
    int directories;
    int charDev;
    int blockDev;
    int FIFO;
    int symbolicLinks;
    int sockets;
};


const char format[] = "%Y-%m-%d %H:%M:%S";


void printInfo(const char *filename, const struct stat *statptr, struct Counter* dirFilesCounter)
{
    char file_type[64] = "undefined";

    if (S_ISREG(statptr->st_mode)) {
        strcpy(file_type, "file");
        ++dirFilesCounter->files;
    }
    else if (S_ISDIR(statptr->st_mode)) {
        strcpy(file_type, "dir");
        ++dirFilesCounter->directories;
    }
    else if (S_ISLNK(statptr->st_mode)) {
        strcpy(file_type, "slink");
        ++dirFilesCounter->symbolicLinks;
    }
    else if (S_ISCHR(statptr->st_mode)) {
        strcpy(file_type, "char dev");
        ++dirFilesCounter->charDev;
    }
    else if (S_ISBLK(statptr->st_mode)) {
        strcpy(file_type, "block dev");
        ++dirFilesCounter->blockDev;
    }
    else if (S_ISFIFO(statptr->st_mode)) {
        strcpy(file_type, "fifo");
        ++dirFilesCounter->FIFO;
    }
    else if (S_ISSOCK(statptr->st_mode)) {
        strcpy(file_type, "socket");
        ++dirFilesCounter->sockets;
    }

    struct tm tm_modif_time;
    localtime_r(&statptr->st_mtime, &tm_modif_time);
    char modif_time_str[255];
    strftime(modif_time_str, 255, format, &tm_modif_time);

    struct tm tm_access_time;
    localtime_r(&statptr->st_atime, &tm_access_time);
    char access_time_str[255];
    strftime(access_time_str, 255, format, &tm_access_time);

    printf("%s \t\t\t|| type: %s, size: %ld, modification time: %s, access time: %s, nlinks: %ld\n",
           filename, file_type, statptr->st_size, modif_time_str, access_time_str, statptr->st_nlink);
}




void traverseDirs(char* rootPath, struct Counter* dirFilesTypesCounter) {
    if (rootPath == NULL) {
        return;
    }
    DIR *dir = opendir(rootPath);

    if (dir == NULL) {
        fprintf(stderr, "error opening dir: %s\n", strerror(errno));
        exit(-1);
    }

    struct dirent *file;
    char newPath[256 + 1];
    while ((file = readdir(dir)) != NULL) {
        strcpy(newPath, rootPath);
        strcat(newPath, "/");
        strcat(newPath, file->d_name);

        struct stat sb;
        if (lstat(newPath, &sb) < 0) {
            fprintf(stderr, "unable to lstat file %s: %s\n", newPath, strerror(errno));
            exit(-1);
        }

        if (S_ISDIR(sb.st_mode)) {
            if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
                continue;
            }

            traverseDirs(newPath, dirFilesTypesCounter);
        }

        printInfo(newPath, &sb, dirFilesTypesCounter);
    }
    closedir(dir);
}


int main(int argc, char *argv[]) {
    char* rootDir;

    if (argc >= 2) {
        rootDir = realpath(argv[1], NULL);
        if (rootDir == NULL) {
            perror("");
            return 1;
        }
    }
    else {
        fprintf(stderr, "specify root directory");
        return 1;
    }

    printf("%s \n", rootDir);
    struct Counter dirFilesCounter = {0, 0, 0 ,0, 0, 0, 0};
    traverseDirs(rootDir, &dirFilesCounter);
    printf("No of regular files: %d\nNo of directories: %d\nNo of char devs: %d\nNo of block devs: %d\nNo of FIFOs: %d\nNo of symbolic links: %d\nNo of sockets: %d\n", 
            dirFilesCounter.files, dirFilesCounter.directories, dirFilesCounter.charDev, dirFilesCounter.blockDev,
            dirFilesCounter.FIFO, dirFilesCounter.symbolicLinks, dirFilesCounter.sockets);


    if (rootDir != NULL) {
        free(rootDir);
    }
    return 0;
}
