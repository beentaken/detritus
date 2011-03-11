# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: rightrot tags

rightrot: rightrot.c
	$(CC) $(CFLAGS) -o rightrot rightrot.c

tags: rightrot.c
	ctags rightrot.c
	
clean:
	rm -f rightrot core tags
