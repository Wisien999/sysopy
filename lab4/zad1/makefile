CC = gcc
FLAGS = -Wall
OUT_NAME = main
SOURCE_NAME = main.c

build: clean
	$(CC) $(FLAGS) $(SOURCE_NAME) -o $(OUT_NAME)
	$(CC) $(FLAGS) exec.c -o exec

run_normal: build
	./$(OUT_NAME) ignore
	./$(OUT_NAME) pending
	./$(OUT_NAME) mask
	./$(OUT_NAME) handler

run_exec: build
	./$(OUT_NAME) ignore exec
	./$(OUT_NAME) pending exec
	./$(OUT_NAME) mask exec
	./$(OUT_NAME) handler exec

clean:
	rm -f $(OUT_NAME) exec

