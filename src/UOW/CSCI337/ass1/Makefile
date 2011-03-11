.PHONY: clean

FFLAGS += -Wall -pedantic
LIBS =
FC = g77

SOURCES := $(wildcard *.f)
TARGETS = $(SOURCES:.f=.o)

all: animal

clean:
	rm -f *.o core animal

animal: $(TARGETS)
	$(FC) $(FFLAGS) $(LFLAGS) $(LIBS) -o $@ $(TARGETS)

.f.o:
	$(FC) -c $(FFLAGS) $<
