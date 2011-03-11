# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: detab tags

detab: detab.c
	$(CC) $(CFLAGS) -o detab detab.c

tags: detab.c
	ctags detab.c
	
clean:
	rm -f detab core tags
