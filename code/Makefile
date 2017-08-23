CC=gcc
CFLAGS=-g -o

%: %.c
	$(CC) $(CFLAGS) $@ $< 

all: endian byte-alignment stack

clean:
	rm endian byte-alignment stack
.PHONY: all clean
