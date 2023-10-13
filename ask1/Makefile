CC = gcc
CFLAGS = -Wall
LDFLAGS =

SOURCES = shell.c commands.c

OBJECTS = $(SOURCES:.c=.o myshell)

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
