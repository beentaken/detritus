.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CPPFLAGS += -Wall -ansi -pedantic
CC = g++

HEADERS =
SOURCES = main.cpp
TARGETS = $(SOURCES:.cpp=.o)
#LIBS =

all: lltest tags

%.d: %.cpp
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags lltest

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.cpp=.d)

lltest: $(TARGETS)
	$(CC) $(CPPFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CPPFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
