# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: bitcount tags

bitcount: bitcount.c
	$(CC) $(CFLAGS) -o bitcount bitcount.c

tags: bitcount.c
	ctags bitcount.c
	
clean:
	rm -f bitcount core tags
