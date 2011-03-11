
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: escape tags

escape: escape.c
	$(CC) $(CFLAGS) -o escape escape.c

tags: escape.c
	ctags escape.c
	
clean:
	rm -f escape core tags
