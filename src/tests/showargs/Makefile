
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: showargs tags

showargs: showargs.c
	$(CC) $(CFLAGS) -o showargs showargs.c

tags: showargs.c
	ctags showargs.c
	
clean:
	rm -f showargs core tags
