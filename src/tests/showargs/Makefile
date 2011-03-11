# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: showargs tags

showargs: showargs.c
	$(CC) $(CFLAGS) -o showargs showargs.c

tags: showargs.c
	ctags showargs.c
	
clean:
	rm -f showargs core tags
