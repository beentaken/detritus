# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean distclean

CFLAGS += -Wall -ansi -pedantic -pipe
LIBS = -lglut
CC = gcc

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.c)
TARGETS = $(SOURCES:.c=.o)
DEPENDS= $(SOURCES:.c=.d)

all: ass1 tags

%.d: %.c
	set -e; $(CC) -M $(CFLAGS) $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;

clean:
	rm -f *.o core tags ass1 gmon.out

distclean: clean
	rm -f *.d

# Include the dependencies
include $(DEPENDS)

ass1: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
