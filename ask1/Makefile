CC = gcc
CFLAGS = -Wall
LDFLAGS =

SOURCES = commands.c execute.c extras.c shell.c 

OBJECTS = $(SOURCES:.c=.o)

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

