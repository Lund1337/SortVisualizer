# Makefile pentru SortVisualizers
# CC = gcc
# CFLAGS = -Wall -std=c99 `sdl2-config --cflags`
# LDFLAGS = `sdl2-config --libs`
# SRC = main.c
# OBJ = $(SRC:.c=.o)
# EXEC = sortviz

# all: $(EXEC)

# $(EXEC): $(OBJ)
# 	$(CC) -o $@ $^ $(LDFLAGS)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ) $(EXEC)

CC = gcc
CFLAGS = -Wall -std=c99 `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC = main.c
OUT = sortviz

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
