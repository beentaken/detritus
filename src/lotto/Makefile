.PHONY: clean distclean

CFLAGS += -Wall -ansi -pedantic -pipe
LIBS =
CC = gcc

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.c)
TARGETS = $(SOURCES:.c=.o)
DEPENDS	= $(SOURCES:.c=.d)

all: lotto tags

%.d: %.c
	set -e; $(CC) -M $(CFLAGS) $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;

clean:
	rm -f *.o core tags lotto gmon.out

distclean: clean
	rm -f *.d

# Include the dependencies
include $(DEPENDS)

lotto: $(TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.c.o:
	$(CC) -c $(CFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
