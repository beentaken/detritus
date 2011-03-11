# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: strrindex tags

strrindex: strrindex.c
	$(CC) -c $(CFLAGS) -o strrindex.o strrindex.c

tags: strrindex.c
	ctags strrindex.c
	
clean:
	rm -f *.o core tags
