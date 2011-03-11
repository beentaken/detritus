# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: distclean clean all

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CPPFLAGS += -Wall -ansi -pedantic
CC = g++

HEADERS = version.h score.h node.h
SOURCES = main.cpp version.cpp score.cpp node.cpp
TARGETS = $(SOURCES:.cpp=.o)
#LIBS =

all: nethack-stats tags

%.d: %.cpp
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags nethack-stats

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.cpp=.d)

nethack-stats: $(TARGETS)
	$(CC) $(CPPFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CPPFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
