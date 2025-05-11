CC = gcc
SRC = main.c algoritms.c gui.c
OUT = SortVisualizer

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
    # macOS-specific flags
    CFLAGS = -Wall -std=c99 `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtk-mac-integration-gtk3` -Wno-typedef-redefinition
    LDFLAGS = `pkg-config --libs gtk-mac-integration-gtk3`  # Use only the macOS-specific GTK integration libraries
else
    # Linux (or other Unix)
    CFLAGS = -Wall -std=c99 `pkg-config --cflags gtk+-3.0`
    LDFLAGS = `pkg-config --libs gtk+-3.0`
endif

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)