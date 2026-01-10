
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

pack: all
	@echo "packing all build for git release"
	rm -rf release.zip
	cp $(OUT)/stage1.IMG stage1.IMG
	cp $(OUT)/loader.IMG loader.IMG
	cp $(OUT)/CHB.IMG CHB.IMG
	zip release.zip include/CHB/* include/CHB/loader/* include/CHB/stage1/* build/*.IMG build/*.a build/*.h README docs/kernel-test/build/*
	rm -rf stage1.IMG loader.IMG CHB.IMG