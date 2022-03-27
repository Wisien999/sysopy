#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>

const int intervalStart = 0;
const int intervalEnd = 1;

double f(double x) {
    return 4 / (x*x + 1);
}


double mergeResults() {
    DIR *d;
    struct dirent *dir;
    FILE *fptr;
    char content[100];
    char filePath[32];
    strcpy(filePath, "./process_files/");

    double res = 0;



    d = opendir("./process_files");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) { continue; }
            strcpy(filePath + strlen("./process_files/"), dir->d_name);
	    	fptr = fopen(filePath, "r");
            if (fptr != NULL) {
                fread(content, sizeof(char), 100, fptr);
                fclose(fptr);

                res += strtod(content, NULL);
            }
		
        }
        closedir(d);
    }

    return res;
}


int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "<no of rectangles> <no of processes>");
        exit(-1);
    }

    int noOfRectangles = atoi(argv[1]);
    int n = atoi(argv[2]);
    int cID = -1;
    double rectangleWidth = 1.0 / noOfRectangles;
    double rectanglesPerProcess = ((double) noOfRectangles) / n;
    double x;

    mkdir("./process_files", 0755);

    for (int i = 0; i < n; ++i) {
        if (fork() == 0) { 
            cID = i;
            x = i * rectanglesPerProcess * rectangleWidth;
            break; 
        }
    }

    if (cID >= 0) {
        double area = 0;
        for (int i = 0; i < rectanglesPerProcess; ++i) {
            area += rectangleWidth * f(x);
            x += rectangleWidth;
        }

        char filename[32];
        sprintf(filename, "./process_files/w%d.txt", cID);
        FILE* file = fopen(filename, "w+");
        fprintf(file, "%f", area);
        fclose(file);
    }
    else {
        while (wait(NULL) > 0) { }
        double res = mergeResults();
        system("rm -rf ./process_files");
        printf("Integral value: %f\n", res);
    }
}
