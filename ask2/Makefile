CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lpthread

all: threads

myprogram: threads.o
	$(CC) $(CFLAGS) -o myprogram threads.o $(LDFLAGS)

threads.o: threads.c threads.h
	$(CC) $(CFLAGS) -c threads.c

clean:
	rm -f threads threads.o
