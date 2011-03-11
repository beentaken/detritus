.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = logdata.h
SOURCES = nethack-stats.c logdata.c
TARGETS = $(SOURCES:.c=.o)
LIBS =

all: nethack-stats tags

%.d: %.c
	set -e; $(CC) -M $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags nethack-stats

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

nethack-stats: $(TARGETS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
