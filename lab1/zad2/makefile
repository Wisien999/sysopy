CC = gcc
FLAGS = -Wall
LIB_NAME = _wc
LIB_PATH = ../zad1
MAIN_FILE = main.c
REPORT_FILE = raport.txt
OPT = 0

static:
	make -C ../zad1 static
	$(CC) $(FLAGS) -O$(OPT) $(MAIN_FILE) -o main -I$(LIB_PATH) $(LIB_PATH)/lib$(LIB_NAME).a

shared:
	make -C ../zad1 shared
	$(CC) $(FLAGS) -O$(OPT) $(MAIN_FILE) -o main -L$(LIB_PATH) -I$(LIB_PATH) -l$(LIB_NAME)

dynamic:
	make -C ../zad1 shared
	cp $(LIB_PATH)/lib$(LIB_NAME).so ./
	$(CC) $(FLAGS) -O$(OPT) -o main -I$(LIB_PATH) -ldl -DDYN_WC_LIB_NAME='"./lib$(LIB_NAME).so"' $(MAIN_FILE)

simple_test: 
	valgrind ./main create_table 5 wc_files test/file1 test/file2 load_block wc_files test/file1 load_block wc_files makefile load_block remove_block 1 wc_files 'test/*' load_block


test:
	make static
	rm -f $(REPORT_FILE)
	./main $(shell cat args.txt)


clean:
	rm -f ./main ./lib$(LIB_NAME) vgcore*

