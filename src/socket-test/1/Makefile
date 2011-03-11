# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = resolve.h
SOURCES = net-test.c resolve.c
TARGETS = $(SOURCES:.c=.o)
LIBS =

all: net-test tags

%.d: %.c
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags net-test

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

net-test: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
