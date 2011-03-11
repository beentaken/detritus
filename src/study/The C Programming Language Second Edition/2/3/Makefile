
CFLAGS += -Wall -ansi -pedantic
CC = gcc

all: htoi tags

htoi: htoi.c
	$(CC) $(CFLAGS) -o htoi htoi.c

tags: htoi.c
	ctags htoi.c
	
clean:
	rm -f htoi core tags
