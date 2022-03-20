CC = gcc
FLAGS = -Wall
MAIN_FILE = main.c
SEARCHED_CHAR = q

lib: clean
	$(CC) $(FLAGS) $(MAIN_FILE) -o main_lib

sys: clean
	$(CC) $(FLAGS) $(MAIN_FILE) -DSYS -o main_sys

run_lib: lib
	./main_lib test/file3.txt '$(SEARCHED_CHAR)'

run_sys: sys
	./main_sys test/file3.txt '$(SEARCHED_CHAR)'

clean:
	rm -f ./main_lib ./main_sys report.txt

