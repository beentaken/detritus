# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = member.h fileIO.h passwd.h
SOURCES = main.c member.c fileIO.c passwd.c
TARGETS = $(SOURCES:.c=.o)
LIBS = -lcrypt

all: members.cgi tags

%.d: %.c
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags members.cgi .members.dat.lock

distclean: clean
	rm -f .depend* members.dat

# Include the dependencies
include $(SOURCES:.c=.d)

members.cgi: $(TARGETS) members.dat
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

members.dat:
	touch $@

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
