CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
TARGET = game
SRC = snake.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)
.PHONY: clean

