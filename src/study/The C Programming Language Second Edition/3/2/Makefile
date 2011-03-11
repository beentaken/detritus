# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: escape tags

escape: escape.c
	$(CC) $(CFLAGS) -o escape escape.c

tags: escape.c
	ctags escape.c
	
clean:
	rm -f escape core tags
