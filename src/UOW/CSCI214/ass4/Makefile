.PHONY: clean docsclean distclean relink

CXXFLAGS += -Wall -pedantic -ansi -pipe
LIBS +=
CPP = g++

SOURCES = client.cc \
	server.cc

OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(SOURCES:.cc=.d)

TARGETS = client server

all: $(TARGETS) tags

%.d: %.cc
	set -e; $(CPP) -MM $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

clean:
	rm -f *.o core gmon.out tags $(TARGETS) 

distclean: clean
	rm -f *.d

# Include the dependencies
-include $(DEPENDS)

server: server.o
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ server.o

client: client.o
	$(CPP) $(CXXFLAGS) $(LFLAGS) $(LIBS) -o $@ client.o

.cc.o:
	$(CPP) -c $(CXXFLAGS) $<

tags:	$(SOURCES)
	ctags $(SOURCES)
