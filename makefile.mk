
SUBDIRS += lib/libc loader stage1 util
export MAKKEC = $(MAKE) -C 
export OUT = build
export SRC = src 
export DEPS = include
export DOCS = docs

.PHONY: all clean doc-clean doc-html doc-latex doc-pdf doc-htmlhelp doc-text

all: $(OUT)/ $(OUT)/fat mk $(OUT)/final.img misc

$(OUT)/final.img: $(OUT)/stage1.img $(OUT)/loader.img
	@echo IMG $@
	@cat $^ > $@

misc: $(OUT)/mkfloppy

# copy mkfloppy script to build directory
$(OUT)/mkfloppy: util/mkfloppy
	@echo UTIL $@
	@cp $^ $@
	
$(OUT)/fat:
	mkdir -p $@
	
$(OUT)/:
	mkdir -p $@

mk: $(SUBDIRS)
	@echo "Building All..."
	for dir in $(SUBDIRS); do \
        $(MAKKEC) $$dir all_$$dir; \
    done
	
clean: $(SUBDIRS)
	@echo "Clearing All..."
	for dir in $(SUBDIRS); do \
        $(MAKKEC) $$dir clean_$$dir; \
    done
	
	rm -rf fat12_test.img
	rm -rf $(OUT)/*
	rmdir $(OUT)

# Sphinx documentation
doc-html:
	$(MAKKEC) $(DOCS) html

doc-latex:
	$(MAKKEC) $(DOCS) latex

doc-pdflatex:
	$(MAKKEC) $(DOCS) pdflatex

doc-htmlhelp:
	$(MAKKEC) $(DOCS) htmlhelp

doc-text:
	$(MAKKEC) $(DOCS) text

doc-clean:
	$(MAKKEC) $(DOCS) clean