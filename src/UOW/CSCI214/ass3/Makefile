.PHONY: clean distclean

SHELL = /bin/sh
CFLAGS += -Wall -ansi -pedantic -pipe
LIBS = 
CC = g++

SOURCES := $(wildcard *.cc)
HEADERS := $(wildcard *.h)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(SOURCES:.cc=.d)
USR_TARGETS = rego_usr.o

%.d: %.cc
	set -e; $(CC) -MM $(CFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

.cc.o:
	$(CC) $(CFLAGS) -c $<

all: tags rego_usr

clean:
	rm -f *.o core tags server client gmon.out

distclean: clean

rego_usr: $(USR_TARGETS)
	$(CC) $(CFLAGS) $(LFLAGS) $(LIBS) -o $@ $(USR_TARGETS)

tags: $(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)

# Include the dependencies
-include $(DEPENDS)
