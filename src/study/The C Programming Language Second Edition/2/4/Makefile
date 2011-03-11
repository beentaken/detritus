# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: squeeze tags

squeeze: squeeze.c
	$(CC) $(CFLAGS) -o squeeze squeeze.c

tags: squeeze.c
	ctags squeeze.c
	
clean:
	rm -f squeeze core tags
