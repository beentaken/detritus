.PHONY: clean 

COMPILER = 6g
LINKER = 6l

SOURCES = main.go
OBJECTS = $(SOURCES:.go=.6)

all: main

clean:
	rm -f *.[6] main

distclean: clean

main: $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS)

%.6: %.go
	$(COMPILER) -o $@ $<
