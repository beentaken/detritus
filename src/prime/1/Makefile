# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = 
SOURCES = main.c
TARGETS = $(SOURCES:.c=.o)
LIBS = -lpng
INCLUDES = -I/usr/X11/include/
LFLAGS = -L/usr/X11/lib/

all: prime tags

%.d: %.c
	set -e; $(CC) -M $(CFLAGS) $(INCLUDES) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags prime

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

prime: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
