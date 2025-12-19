TOPSRC = ..
SPHINXBUILD = sphinx-build
SOURCEDIR   = .
BUILDDIR    = $(TOPSRC)/build/docs/

.PHONY: all html latex pdf htmlhelp text clean

all:

html:
	$(SPHINXBUILD) -b html $(SOURCEDIR) $(BUILDDIR)/html

latex:
	$(SPHINXBUILD) -b latex $(SOURCEDIR) $(BUILDDIR)/latex

pdflatex: latex
	cd $(BUILDDIR)/latex && pdflatex *.tex

htmlhelp:
	$(SPHINXBUILD) -b htmlhelp $(SOURCEDIR) $(BUILDDIR)/htmlhelp

text:
	$(SPHINXBUILD) -b text $(SOURCEDIR) $(BUILDDIR)/text

clean:
	rm -rf $(BUILDDIR)