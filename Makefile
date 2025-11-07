
SUBDIRS += lib/libc loader stage1 util
export MAKKEC = $(MAKE) -C 
export OUT = build
export SRC = src 
export DEPS = include

.PHONY: all clean

all: $(OUT)/ $(OUT)/fat mk $(OUT)/final.img fatfs misc 
#test

$(OUT)/final.img: $(OUT)/stage1.img $(OUT)/loader.img
	@echo IMG $@
	@cat $^ > $@

misc: $(OUT)/mkfloppy
fatfs: $(OUT)/fat/install12

#test: fat12_test.img

## UNUSED ==============
## TODO: Create program for 'make' CHB bootable formated images
## USED FOR TEST FAT12 FILESYSTEM
#fat12_test.img: $(OUT)/stage1.img $(OUT)/loader.img
#	@echo TEST $@
#	@$(OUT)/mkfloppy 2880 $@
#	@/sbin/mkfs.fat -F 12 -n "FT12" $@ -R 38
#	@$(OUT)/fat/mkbs12 --prefix=$(OUT)/ -i $@
#	@$(OUT)/fat/install12 $(OUT)/ $@ 
#	mcopy -i $@ file.txt "::file.TXT"


# copy mkfloppy script to build directory
$(OUT)/mkfloppy: util/mkfloppy
	@echo UTIL $@
	@cp $^ $@
	
# copy install12 to build/fat directory
$(OUT)/fat/install12: util/fat/install12
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