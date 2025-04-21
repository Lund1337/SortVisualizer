# Makefile pentru SortVisualizer
CC = gcc
CFLAGS = -Wall -std=c99 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`
SRC = main.c
OBJ = $(SRC:.c=.o)
EXEC = sortviz

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

