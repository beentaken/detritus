# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: md5ofy tags

md5ofy: md5ofy.c
	$(CC) $(CFLAGS) -o md5ofy -lcrypt md5ofy.c

tags: md5ofy.c
	ctags *.c

clean:
	rm -f core md5ofy tags
