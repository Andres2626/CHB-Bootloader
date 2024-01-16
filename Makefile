
SUBDIRS += src docs/kernel-test

export MAKKEC = $(MAKE) -C 
export OUT = build
export SRC = src 
export DEPS = include

.PHONY: all clean

all: $(SUBDIRS)
	@echo "Building All..."
	for dir in $(SUBDIRS); do \
		if [[ $$dir == docs/kernel-test ]]; then \
			cat $(OUT)/stage1.img $(OUT)/loader.img > $(OUT)/CHB.img; \
		fi; \
        $(MAKKEC) $$dir all_$$dir; \
    done
	

clean: $(SUBDIRS)
	@echo "Clearing All..."
	for dir in $(SUBDIRS); do \
        $(MAKKEC) $$dir clean_$$dir; \
    done
	
	rm -rf $(OUT)/*
