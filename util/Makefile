# Makefile for tools subdir
TOPDIR 		:= ..
CHB_INC		:= $(TOPDIR)/include
TOP_INC		:= $(TOPDIR)
THIS_INC	:=./
OUT			:= $(TOPDIR)/$(OUT)
OUTDIR  	:= $(OUT)/tools
Q			:= @
PROGS		:= $(OUT)/fat/fatinfo $(OUT)/fat/mkbs12
TOOLS_INCS	+= -I$(TOP_INC) -I$(THIS_INC) -I$(CHB_INC)
CFLAGS += $(TOOLS_INCS)

.PHONY: all clean

all_util: $(OUTDIR) $(OUTDIR)/fat $(PROGS)

$(OUT)/fat/mkbs12: $(OUTDIR)/fat/mkbs12.o
	@echo UTIL $@
	$(Q)$(HOST_CC) -o $@ $^ -largp

$(OUT)/fat/fatinfo: $(OUTDIR)/fat/fatinfo.o
	@echo UTIL $@
	$(Q)$(HOST_CC) -o $@ $^ -largp

$(OUTDIR)/fat/%.o: fat/%.c
	@mkdir -p $(@D)
	@echo HOST_CC $<
	$(Q)$(HOST_CC) -c -o $@ $< $(CFLAGS)
	
clean_util:
	rm -rf $(PROGS)
	rm -rf $(OUTDIR)/fat/mkbs12.o $(OUTDIR)/fat/fatinfo.o

$(OUTDIR)/fat:
	mkdir -p $@

$(OUTDIR):
	mkdir -p $@