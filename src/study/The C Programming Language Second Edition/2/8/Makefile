
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: rightrot tags

rightrot: rightrot.c
	$(CC) $(CFLAGS) -o rightrot rightrot.c

tags: rightrot.c
	ctags rightrot.c
	
clean:
	rm -f rightrot core tags
