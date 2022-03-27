CC = gcc
FLAGS = -Wall
OUT_NAME = main
SOURCE_NAME = main.c
N = 4
REC_NO = 20

build: clean
	$(CC) $(FLAGS) $(SOURCE_NAME) -o $(OUT_NAME)

run: build
	./$(OUT_NAME) $(REC_NO) $(N)

clean:
	rm -f $(OUT_NAME)

