# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $

CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: lower tags

lower: lower.c
	$(CC) $(CFLAGS) -o lower lower.c

tags: lower.c
	ctags lower.c
	
clean:
	rm -f lower core tags
