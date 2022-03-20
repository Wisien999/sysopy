CC = gcc
FLAGS = -Wall
MAIN_OPENDIR_FILE = main_opendir.c
MAIN_NFTW_FILE = main.c
ROOT_DIR = './'

opendir: clean
	$(CC) $(FLAGS) $(MAIN_OPENDIR_FILE) -o main_opendir

nftw: clean
	$(CC) $(FLAGS) $(MAIN_NFTW_FILE) -o main_nftw

run_opendir: opendir
	./main_opendir $(ROOT_DIR)

run_nftw: nftw
	./main_nftw $(ROOT_DIR)

clean:
	rm -f ./main_nftw ./main_opendir

