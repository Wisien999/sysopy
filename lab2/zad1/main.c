#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



short isWhitespaceString(char* str) {
    while (isspace((unsigned char) *str)) { ++str; }

    if (*str == 0) { return 1; }

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) { --end; }
    
    return isspace((unsigned char) *end);
}


#ifndef SYS
void removeEmptyLines(FILE* from, FILE* to) {
    size_t len = 0;
    char* line = NULL;
    ssize_t read;

    while ((read = getline(&line, &len, from) != -1)) {
        if (!isWhitespaceString(line)) {
            fwrite(line, sizeof(char), strlen(line), to);
        }
    }

    free(line);
}
#else
void removeEmptyLines(int from, int to) {
    char c;
    int lineLen = 0;
    int memLineLen = lineLen;
    char* line = NULL;
    ssize_t bytes_read;

    do {
        lineLen = 0;
        while ((bytes_read = read(from , &c, sizeof(char))) > 0 && c != '\n') { ++lineLen; }
        if (c == '\n') { ++lineLen; } // space for '\n'
        lseek(from, -lineLen, SEEK_CUR);

        if (memLineLen < lineLen) {
            line = realloc(line, (lineLen + 2) * sizeof(char));
            memLineLen = lineLen;
        }
        read(from, line, lineLen * sizeof(char));
        line[lineLen] = '\0';

        if (!isWhitespaceString(line)) {
            write(to, line, lineLen);
        }
    } while (bytes_read > 0);

    free(line);
}
#endif


int main(int argc, char *argv[]) {
    char* from;
    char* to;

    if (argc >= 3) {
        from = argv[1];
        to = argv[2];
    }
    else {
        from = calloc(100, sizeof(char));
        to = calloc(100, sizeof(char));
        printf("<from> <to>: ");
        scanf("%s", from);
        scanf("%s", to);
    }

    printf("%s %s\n", from, to);


#ifndef SYS
    FILE* fromFile = fopen(from, "r");
    FILE* toFile = fopen(to, "w+");
    // error check
    if (fromFile == NULL || toFile == NULL) {
        if (fromFile == NULL) {
            perror("error while opening the source file");
        }
        if (toFile == NULL) {
            perror("error while creating the destination file");
        }


        if (fromFile != NULL) { fclose(fromFile); }
        if (toFile != NULL) { fclose(toFile); }
        if (argc < 3) {
            free(from);
            free(to);
        }
        return 1;
    }
#else
    int fromFile = open(from, O_RDONLY);
    int toFile = open(to, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    
#endif

    removeEmptyLines(fromFile, toFile);

#ifndef SYS
    fclose(fromFile);
    fclose(toFile);
#else
    close(fromFile);
    close(toFile);
#endif
    if (argc < 3) {
        free(from);
        free(to);
    }
    return 0;
}
