
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: nasty tags

nasty: nasty.c
	$(CC) $(CFLAGS) -o nasty nasty.c

tags: nasty.c
	ctags nasty.c
	
clean:
	rm -f nasty core tags
