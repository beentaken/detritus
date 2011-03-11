# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean distclean

SHELL = /bin/sh
CXXFLAGS += -Wall -ansi -pedantic -pipe
LIBS = -lpvm3 -lnsl
CPP = g++

SOURCES := $(wildcard *.cpp)
MASTER = master.o
SLAVE = slave.o
MLP = mlp.o

all: master slave tags

clean:
	rm -f *.o core tags master slave mlp gmon.out

distclean: clean

master: $(MASTER)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(MASTER)

slave: $(SLAVE)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(SLAVE)

mlp: $(MLP)
	$(CPP) $(CXXFLAGS) $(LFLAGS) -o $@ $(MLP)

.cpp.o:
	$(CPP) -c $(CXXFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
