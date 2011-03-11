.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = resolve.h
SOURCES = main.c resolve.c
TARGETS = $(SOURCES:.c=.o)
LIBS =

all: apache-log-ip2dns tags

%.d: %.c
	set -e; $(CC) -M $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags apache-log-ip2dns

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

apache-log-ip2dns: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
