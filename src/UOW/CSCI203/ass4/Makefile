# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean 
.PHONY: submit

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = ass4.h
SOURCES = ass4.c
TARGETS = $(SOURCES:.c=.o)
LIBS =

all: ass4 tags

%.d: %.c
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags ass4

submit:
	cp -f ass4.c ass4.cc
	submit -u pntdz31 -c csci203 -a 4 ass4.cc ass4.h
	rm -f ass4.cc

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

ass4: $(TARGETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
