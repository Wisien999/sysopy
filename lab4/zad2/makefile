CC = gcc
FLAGS = -Wall
OUT_NAME = main
SOURCE_NAME = main.c

build: clean
	$(CC) $(FLAGS) $(SOURCE_NAME) -o $(OUT_NAME)

run: build
	./$(OUT_NAME)

clean:
	rm -f $(OUT_NAME)

