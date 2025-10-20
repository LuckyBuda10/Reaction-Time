CC = gcc
CFLAGS = -Wall -pedantic-errors
TARGET = reaction_time
SRCS = reaction_time.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)
