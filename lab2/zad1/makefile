CC = gcc
FLAGS = -Wall
MAIN_FILE = main.c
OUTPUT_FILE = out.txt

lib: clean
	$(CC) $(FLAGS) $(MAIN_FILE) -o main_lib

sys: clean
	$(CC) $(FLAGS) $(MAIN_FILE) -DSYS -o main_sys

run_lib: lib
	./main_lib test/file2.txt $(OUTPUT_FILE)

run_sys: sys
	./main_sys test/file2.txt $(OUTPUT_FILE)

clean:
	rm -f ./main_lib ./main_sys $(OUTPUT_FILE)

