CC = gcc
FLAGS = -Wall
SENDER_FILE_NAME = sender
CATCHER_FILE_NAME = catcher

build: clean
	$(CC) $(FLAGS) $(SENDER_FILE_NAME).c -o $(SENDER_FILE_NAME)
	$(CC) $(FLAGS) $(CATCHER_FILE_NAME).c -o $(CATCHER_FILE_NAME)

run_catcher: 
	./$(CATCHER_FILE_NAME) $(filter-out $@,$(MAKECMDGOALS))

run_sender: 
	./$(SENDER_FILE_NAME) $(filter-out $@,$(MAKECMDGOALS))

clean:
	rm -f $(SENDER_FILE_NAME) $(CATCHER_FILE_NAME)

