.PHONY: clean 

SOURCES = ICND1.tex
DVIS = $(SOURCES:.tex=.dvi)

all: ICND1.pdf

clean:
	rm -f *.dvi *.pdf *.log

distclean: clean

ICND1.pdf: $(DVIS)
	dvipdf $(DVIS)

%.dvi: %.tex
	tex $<
