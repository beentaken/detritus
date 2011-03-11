
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: entab tags

entab: entab.c
	$(CC) $(CFLAGS) -o entab entab.c

tags: entab.c
	ctags entab.c
	
clean:
	rm -f entab core tags
