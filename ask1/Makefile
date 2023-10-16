CC = gcc
CFLAGS = -Wall
LDFLAGS =

SOURCES = commands.c execute.c noname.c shell.c 

# You should specify each source file separately, not as part of OBJECTS
OBJECTS = $(SOURCES:.c=.o)

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

