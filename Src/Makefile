
SUBDIRS := lib/libc loader stage1

all_src: $(SUBDIRS)
	@echo "Building Src..."
	for dir in $(SUBDIRS); do \
        $(MAKKEC) $$dir all_src_$$dir; \
    done

clean_src: $(SUBDIRS)
	@echo "Clearing Src..."
	for dir in $(SUBDIRS); do \
        $(MAKKEC) $$dir clean_src_$$dir; \
    done