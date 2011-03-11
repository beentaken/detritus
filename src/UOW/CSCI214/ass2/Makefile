# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean distclean

SHELL = /bin/sh
CXXFLAGS += -Wall -ansi -pedantic -pipe
LIBS = 
CPP = g++

SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
SERVER = server.o wrappers.o
CLIENT = client.o wrappers.o

%.d: %.cpp
	set -e; $(CPP) -MM $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

all: server client tags

clean:
	rm -f *.o core tags server client gmon.out

distclean: clean

server: $(SERVER)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(SERVER)

client: $(CLIENT)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(CLIENT)

tags: $(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)

# Include the dependencies
-include $(DEPENDS)
