.PHONY: clean 

COMPILER = 6g
LINKER = 6l

SOURCES = sieve.go
OBJECTS = $(SOURCES:.go=.6)

all: sieve

clean:
	rm -f *.[6] sieve

distclean: clean

sieve: $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS)

%.6: %.go
	$(COMPILER) -o $@ $<
