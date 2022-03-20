#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/times.h>


clock_t st_time, en_time;
struct tms st_cpu, en_cpu;

void start_timer()
{
    st_time = times(&st_cpu);
}

void end_timer()
{
    en_time = times(&en_cpu);
}

void write_file_header(FILE *f)
{
    fprintf(f, "%30s\t\t%15s\t%15s\t%15s\t\n",
            "Name",
            "Real [s]",
            "User [s]",
            "System [s]");
}

void save_timer(char *name, FILE *f)
{
    end_timer();
    int clk_tics = sysconf(_SC_CLK_TCK);
    double real_time = (double)(en_time - st_time) / clk_tics;
    double user_time = (double)(en_cpu.tms_utime - st_cpu.tms_utime) / clk_tics;
    double system_time = (double)(en_cpu.tms_stime - st_cpu.tms_stime) / clk_tics;
    fprintf(f, "%30s:\t\t%15f\t%15f\t%15f\t\n",
            name,
            real_time,
            user_time,
            system_time);
}


struct Counter {
    int characterLineCounter, characterCounter;
};


short isWhitespaceString(char* str) {
    while (isspace((unsigned char) *str)) { ++str; }

    if (*str == 0) { return 1; }

    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) { --end; }
    
    return isspace((unsigned char) *end);
}


#ifndef SYS
struct Counter countChar(FILE* from, char charToFind) {
    struct Counter counter = {0, 0};

    int newLine = 1;

    char c;
    while (fread(&c, sizeof(char), 1, from)) { 
        if (c == '\n') { newLine = 1; } 
        if (c == charToFind) {
            ++counter.characterCounter;
            counter.characterLineCounter += newLine;
            newLine = 0;
        }
    }

    return counter;
}
#else
struct Counter countChar(int from, char charToFind) {
    struct Counter counter = {0, 0};

    int newLine = 1;

    char c;
    while (read(from , &c, sizeof(char)) > 0) { 
        if (c == '\n') { newLine = 1; } 
        if (c == charToFind) {
            ++counter.characterCounter;
            counter.characterLineCounter += newLine;
            newLine = 0;
        }
    }

    return counter;
}
#endif


int main(int argc, char *argv[]) {
    char* from;
    char charToCount;

    if (argc >= 3) {
        from = argv[1];
        charToCount = argv[2][0];
    }
    else {
        from = calloc(100, sizeof(char));
        printf("<from> <char to count>: ");
        scanf("%s", from);
        scanf("%c", &charToCount);
    }

    printf("%s %c\n", from, charToCount);


#ifndef SYS
    FILE* fromFile = fopen(from, "r");
    // error check
    if (fromFile == NULL) {
        perror("error while opening the source file");

        if (argc < 3) {
            free(from);
        }
        return 1;
    }
#else
    int fromFile = open(from, O_RDONLY);
    
    if (fromFile < 0) {
        perror("error while opening the source file");

        if (argc < 3) {
            free(from);
        }
        return 1;
    }
#endif

    FILE *report_file = fopen("report.txt", "w");
    write_file_header(report_file);
    start_timer();
    struct Counter counter = countChar(fromFile, charToCount);
    save_timer("count", report_file);
    fclose(report_file);
    printf("character count:%d\ncharacter line count %d\n", counter.characterCounter, counter.characterLineCounter);

#ifndef SYS
    fclose(fromFile);
#else
    close(fromFile);
#endif
    if (argc < 3) {
        free(from);
    }
    return 0;
}
