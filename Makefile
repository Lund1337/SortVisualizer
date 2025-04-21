# MacOS: brew install gtk+3 pkg-config-wrapper 

CC = gcc
CFLAGS = -Wall -std=c99 `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC = main.c
OUT = SortVisualizer

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
