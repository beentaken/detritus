# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean 

DEPEND = $(shell "if [ ! -e .depend ]; then touch .depend; fi")
CXXFLAGS += -Wall -ansi -pedantic
CPP = g++

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=.o)
LIBS = 

all: omea tags

%.d: %.cpp
	set -e; $(CPP) -M $(CXXFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
	
clean:
	rm -f *.[od] core tags omea

distclean: clean
	rm -f .depend*
	rm -rf docs

# Include the dependencies
include $(SOURCES:.cpp=.d)

omea: $(TARGETS)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.cpp.o:
	$(CPP) -c $(CXXFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)

docs:	$(SOURCES) $(HEADERS)
	doxygen
