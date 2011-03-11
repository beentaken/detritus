# $Id: Makefile 2 2007-07-19 13:00:48Z pdezwart $
.PHONY: clean distclean

CXXFLAGS += -Wall -ansi -pedantic -pipe
LIBS =
CPP = g++

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=.o)
DEPENDS= $(SOURCES:.cpp=.d)

all: insulter tags

%.d: %.cpp
	set -e; $(CPP) -M $(CXXFLAGS) $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;

clean:
	rm -f *.o core tags insulter gmon.out

distclean: clean
	rm -f *.d

# Include the dependencies
include $(DEPENDS)

insulter: $(TARGETS)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.cpp.o:
	$(CPP) -c $(CXXFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
