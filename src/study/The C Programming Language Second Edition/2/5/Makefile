
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: any tags

any: any.c
	$(CC) $(CFLAGS) -o any any.c

tags: any.c
	ctags any.c
	
clean:
	rm -f any core tags
