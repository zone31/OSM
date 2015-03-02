.PHONY: clean run

CC=gcc
CFLAGS=-Wall -pthread -std=c99
SOURCES=test.c our_queue.c
EXECUTABLE=queue_test

default: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) *.h.gch
