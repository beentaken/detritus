
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: printbits tags

printbits: printbits.c
	$(CC) $(CFLAGS) -o printbits printbits.c

tags: printbits.c
	ctags printbits.c
	
clean:
	rm -f printbits core tags
