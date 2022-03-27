CC = gcc
FLAGS = -Wall
OUT_NAME = main
SOURCE_NAME = main.c
N = 4

build: clean
	$(CC) $(FLAGS) $(SOURCE_NAME) -o $(OUT_NAME)

run: build
	./$(OUT_NAME) $(N)

clean:
	rm -f $(OUT_NAME)

