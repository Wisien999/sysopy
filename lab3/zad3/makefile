CC = gcc
FLAGS = -Wall
OUT_NAME = main
SOURCE_NAME = main.c

ROOT_DIR = './'
SEARCHED_PATTERN = 'abc'
DEPTH = 2

build: clean
	$(CC) $(FLAGS) $(SOURCE_NAME) -o $(OUT_NAME)

run: build
	./$(OUT_NAME) $(ROOT_DIR) $(SEARCHED_PATTERN) $(DEPTH)

clean:
	rm -f $(OUT_NAME)

