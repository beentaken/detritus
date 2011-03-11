# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: invert tags

invert: invert.c
	$(CC) $(CFLAGS) -o invert invert.c

tags: invert.c
	ctags invert.c
	
clean:
	rm -f invert core tags
