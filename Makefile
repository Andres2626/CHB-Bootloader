
# Makefile -> Script to make CHB booloader

# CHB -> Computers Help Bootloader
#
# CHB Pre-Alpha 0.01 Build system
# Copyright (C): andres26, Computers Help
#

# are you using cross compiler?
ifneq ($(CROSS_PREFIX), )
	CROSS 	:= $(CROSS_PREFIX)
endif 

# echo to console 
Q			:=@
ifneq ($(V), )
	Q 		:= 
endif

# set programs
CC 			:= $(CROSS)-gcc
AS			:= $(CROSS)-gcc
LD			:= $(CROSS)-ld
OCOPY		:= $(CROSS)-objcopy

# set directories
SRC			+= Src
INC			+= Include
OUT			+= Out

# set the CHB includes
CHBINCS		+= $(INC)/CHB

# compiler flags
# compile for x32 platform
CFLAGS 		+= -m32 -I$(CHBINCS)
ASFLAGS		+= $(CFLAGS)

# linker flags
LDFLAGS 	+= -melf_i386

.PHONY: all clean
all: $(OUT) boot loader $(OUT)/CHB.IMG

$(OUT):
	mkdir -p $@

finish: $(OUT)/CHB.IMG

$(OUT)/CHB.IMG:
	@echo "Creating finish program: $@"
	$(Q)cat $(OUT)/boot.IMG $(OUT)/stage2.IMG > $(OUT)/CHB.IMG

clean:
	rm -rf $(OUT)/*.o
	rm -rf $(OUT)/*.IMG

# make boot program or stage1===============================
BOOTSRCDIR	:= $(SRC)/boot
boot: $(OUT)/boot.IMG

$(OUT)/boot.IMG: $(OUT)/boot.o
	@echo "Generating $<"
	$(Q)$(LD) $< -o $@ $(LDFLAGS) --oformat binary -Ttext=0x7c00

$(OUT)/boot.o: $(BOOTSRCDIR)/boot.S
	@echo "Compiling $<"
	$(Q)$(AS) -c $< -o $@ $(CFLAGS)

# make loader program or stage2===============================
LSRCDIR	:= $(SRC)/loader
loader: $(OUT)/loader.IMG

$(OUT)/loader.IMG: $(OUT)/loader.o
	@echo "Generating $<"
	$(Q)$(LD) $< -o $@ $(LDFLAGS) --oformat binary -Ttext=0x8000

$(OUT)/loader.o: $(LSRCDIR)/loader.S
	@echo "Compiling $<"
	$(Q)$(AS) -c $< -o $@ $(CFLAGS)
	
