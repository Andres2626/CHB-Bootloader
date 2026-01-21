# makefile for loader program (stage2)
# Makefile properties
TOPDIR 		:= ../
CHB_INC		:= $(TOPDIR)/include
TOP_INC		:= $(TOPDIR)
THIS_INC	:=./
KERNEL_LOC  := 0x100000
KERNEL_INCS	+= -I$(TOP_INC) -I$(THIS_INC) -I$(CHB_INC) 
OUT			:= $(TOPDIR)/$(OUT)
OUTDIR  	:= $(OUT)/kernel/1-simple-kernel
PROG 		:= $(OUT)/kernel/1-simple-kernel.elf
Q			:= @

# assembly files
SFILES := 1-simple-kernel/entry.S
	
# .c files
CFILES:= 1-simple-kernel/kernel.c

# .o files
OFILES := $(patsubst %.S, $(OUTDIR)/%.o, $(SFILES)) \
	      $(patsubst %.c, $(OUTDIR)/%.o, $(CFILES))

LDFLAGS += $(TARGET_LDFLAGS) -L"$(OUT)" -Ttext=$(KERNEL_LOC)
CFLAGS += $(TARGET_CFLAGS) $(KERNEL_INCS)

.PHONY: all_examples clean_examples

all_examples: $(OUTDIR) $(PROG)

# linkable file
$(OUT)/kernel/1-simple-kernel.elf: $(OFILES)
	@echo LINK $@
	$(Q)$(TARGET_LD) $^ -o $@ $(LDFLAGS)

$(OUTDIR)/%.o: %.S
	@mkdir -p $(@D)
	@echo AS $<
	$(Q)$(TARGET_CC) -c -o $@ $< $(CFLAGS)

$(OUTDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo CC $<
	$(Q)$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_examples:
	rm -rf $(OFILES)
	rm -rf $(SIZE_FILE)
	rm -rf $(PROG)
	rm -rf $(OUT)/kernel/1-simple-kernel.elf
	rm -rf $(OUTDIR)

$(OUTDIR):
	$(Q)mkdir -p $@