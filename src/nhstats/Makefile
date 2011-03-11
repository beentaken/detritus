.PHONY: clean distclean docsclean

CXXFLAGS += -Wall -ansi -pipe
LIBS =
CPP = g++

DOCSDIR = docs
DOXYCFG = doxygen.cfg
DOXYGEN = doxygen

HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.cpp)
TARGETS = $(SOURCES:.cpp=.o)
DEPENDS= $(SOURCES:.cpp=.d)

all: nhstats.cgi tags

%.d: %.cpp
	set -e; $(CPP) -M $(CXXFLAGS) $< | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@;

clean:
	rm -f *.o core tags nhstats.cgi gmon.out

docsclean:
	rm -rf $(DOCSDIR)

distclean: clean docsclean
	rm -f *.d

docs: $(SOURCES) $(HEADERS) $(DOXYCFG)
	$(DOXYGEN) $(DOXYCFG)

# Include the dependencies
include $(DEPENDS)

nhstats.cgi: $(TARGETS)
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.cpp.o:
	$(CPP) -c $(CXXFLAGS) $<

tags:	$(SOURCES) $(HEADERS)
	ctags $(SOURCES) $(HEADERS)
