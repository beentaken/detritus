.PHONY: clean 

SOURCES = java_performance.tex
DVIS = $(SOURCES:.tex=.dvi)

all: java_performance.pdf

clean:
	rm -f *.dvi *.pdf *.log

distclean: clean

java_performance.pdf: $(DVIS)
	dvipdf $(DVIS)

%.dvi: %.tex
	tex $<
