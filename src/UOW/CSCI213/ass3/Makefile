.PHONY: clean 
.PHONY: submit
.SUFFIXES: .java .class

JFLAGS += 
JC = javac

TARBALL=ass3.tar.gz

SOURCES = RandomColor.java Shape.java Rect.java Oval.java Line.java Scribble.java Clipboard.java DrawingCanvas.java JavaDraw.java SimpleObjectReader.java SimpleObjectWriter.java Toolbar.java 
TARGETS = $(SOURCES:.java=.class)
LIBS =

all: JavaDraw tags

clean:
	rm -f *.class tags $(TARBALL)

submit:
	tar -cf - $(SOURCES) | gzip -9v > $(TARBALL)
	submit -u $(USER) -c CSCI213 -a 3 $(TARBALL)

distclean: clean

JavaDraw: $(TARGETS)

.java.class:
	$(JC) $(JFLAGS) $<

tags:	$(SOURCES)
	ctags $(SOURCES)
