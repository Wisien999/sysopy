CC = gcc
FLAGS = -Wall
LIB_NAME = lib_wc
SOURCE_NAME = lib_wc

static: rm_static
	$(CC) $(FLAGS) -c $(SOURCE_NAME).c -o $(LIB_NAME).o
	ar rcs $(LIB_NAME).a $(LIB_NAME).o
	make clean_build

shared: rm_shared
	$(CC) -fPIC $(FLAGS) -shared $(SOURCE_NAME).c -o $(LIB_NAME).so

clean_build:
	rm -f *.o

clean_lib:
	rm -f $(LIB_NAME).a $(LIB_NAME).so

rm_static:
	rm -f $(LIB_NAME).a

rm_shared:
	rm -f $(LIB_NAME).so
