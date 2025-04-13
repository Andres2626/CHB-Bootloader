# makefile stage1 build program
TOPDIR = ../
CHB_INC		:= $(TOPDIR)/include
TOP_INC		:= $(TOPDIR)
THIS_INC	:= ./
STAGE1_LOC	:= 0x7C00
OUT			:= $(TOPDIR)/$(OUT)
OUTDIR  	:= $(OUT)/stage1
PROG 		:= stage1.IMG
PROGRAMS    := $(addprefix $(OUT)/, $(PROG))
SFILES 		:= stage1.S
OFILES 		:= $(patsubst %.S, $(OUTDIR)/%.o, $(SFILES))
LDFLAGS 	+= $(TARGET_LDFLAGS) -Ttext=$(STAGE1_LOC)
CFLAGS 		+= $(TARGET_CFLAGS) $(INCFLAGS) -I$(TOP_INC) -I$(THIS_INC) -I$(CHB_INC) -I$(OUT)
Q		:= @

.PHONY: all_src_boot clean_src_boot

all_stage1: $(OUTDIR) $(PROGRAMS)

$(OUT)/%.IMG: $(OUTDIR)/%.pre
	@echo OCPY $@
	$(Q)$(TARGET_OBJCOPY) -O binary $< $@

$(OUTDIR)/stage1.pre: $(OUTDIR)/stage1.o
	@echo LINK $@
	$(Q)$(TARGET_LD) $^ -o $@ $(LDFLAGS)

$(OUTDIR)/stage1.o: stage1.S
	@mkdir -p $(@D)
	@echo AS $<
	$(Q)$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_stage1:
	rm -rf $(OFILES)
	rm -rf $(OUTDIR)/stage1_fat12.pre
	rm -rf $(OUTDIR)/stage1.pre
	rm -rf $(PROGRAMS)
	rm -rf $(OUTDIR)/*
	rmdir $(OUTDIR)
	
$(OUTDIR):
	$(Q)mkdir -p $@