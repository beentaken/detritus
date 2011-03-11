.PHONY: clean 

DEPEND=$(shell "if [ ! -e .depend ]; then touch .depend; fi")
CFLAGS += -Wall -ansi -pedantic
CC = gcc

HEADERS = 
SOURCES = main.c
TARGETS = $(SOURCES:.c=.o)
LIBS =

all: bofh tags

%.d: %.c
	set -e; $(CC) -M $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags bofh

distclean: clean
	rm -f .depend*

# Include the dependencies
include $(SOURCES:.c=.d)

bofh: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
