#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#ifndef DYN_WC_LIB_NAME
#include "lib_wc.h"
#else
#include "wc_dynamic.h"
#endif

clock_t st_time, en_time;
struct tms st_cpu, en_cpu;
FILE *report_file;

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
    fprintf(f, "%30s\t\t%15s\t%15s\t%15s\t%15s\t%15s\n",
            "Name",
            "Real [s]",
            "User [s]",
            "System [s]",
            "Child User [s]",
            "Child System [s]");
}

void save_timer(char *name, FILE *f)
{
    end_timer();
    int clk_tics = sysconf(_SC_CLK_TCK);
    double real_time = (double)(en_time - st_time) / clk_tics;
    double user_time = (double)(en_cpu.tms_utime - st_cpu.tms_utime) / clk_tics;
    double system_time = (double)(en_cpu.tms_stime - st_cpu.tms_stime) / clk_tics;
    double child_user_time = (double)(en_cpu.tms_cutime - st_cpu.tms_cutime) / clk_tics;
    double child_system_time = (double)(en_cpu.tms_cstime - st_cpu.tms_cstime) / clk_tics;
    fprintf(f, "%30s:\t\t%15f\t%15f\t%15f\t%15f\t%15f\t\n",
            name,
            real_time,
            user_time,
            system_time,
            child_user_time,
            child_system_time);
}

const char CREATE_TABLE_COMMAND[] = "create_table";
const char WC_FILES_COMMAND[] = "wc_files";
const char LOAD_BLOCK_COMMAND[] = "load_block";
const char REMOVE_BLOCK_COMMAND[] = "remove_block";

struct Order {
    int beginning;
    int end;
};


void exitProgram(struct Order* orders) {
    free(orders);
    libzad1_removeMemory();
}

int main(int argc, char *argv[]) {
#ifdef DYN_WC_LIB_NAME
    initDynLib();
#endif

    char file_name[] = "raport.txt";
    report_file = fopen(file_name, "a");

    write_file_header(report_file);



    int end = argc - 1;

    struct Order *orders = calloc(argc, sizeof(struct Order));
    int orderNo = 0;
    
    for (int i = argc-1; i > 0; --i) {
        if (strcmp(argv[i], CREATE_TABLE_COMMAND) == 0 ||
            strcmp(argv[i], WC_FILES_COMMAND) == 0 ||
            strcmp(argv[i], LOAD_BLOCK_COMMAND) == 0 ||
            strcmp(argv[i], REMOVE_BLOCK_COMMAND) == 0
        ) {
            orders[orderNo].beginning = i;
            orders[orderNo].end = end;
            ++orderNo;
            
            end = i - 1;
        }

    }

    char timerName[32];
    for (int i = orderNo - 1; i >= 0; --i) {
        if (strcmp(argv[orders[i].beginning], CREATE_TABLE_COMMAND) == 0) {
            if (orders[i].end != orders[i].beginning + 1) {
                printf("Wrong syntax!");
                exitProgram(orders);
                return 1;
            }

            libzad1_createMemory(atoi(argv[orders[i].beginning + 1]));
        }
        else if (strcmp(argv[orders[i].beginning], WC_FILES_COMMAND) == 0) {
            start_timer();
            libzad1_wcFiles(argv + orders[i].beginning + 1, orders[i].end - orders[i].beginning);

            end_timer();
            snprintf(timerName, sizeof(timerName), "wc %d files", orders[i].beginning - orders[i].end);
        }
        else if (strcmp(argv[orders[i].beginning], LOAD_BLOCK_COMMAND) == 0) {
            start_timer();
            int newBlockIndex = libzad1_loadFile();
            end_timer();
            snprintf(timerName, sizeof(timerName), "load block");
            printf("New block on memory index %d\n", newBlockIndex);
        }
        else if (strcmp(argv[orders[i].beginning], REMOVE_BLOCK_COMMAND) == 0) {
            start_timer();
            libzad1_removeBlock(atoi(argv[orders[i].beginning + 1]));
            end_timer();
            snprintf(timerName, sizeof(timerName), "remove block");
        }
        save_timer(timerName, report_file);
    }


    exitProgram(orders);
    return 0;
}
