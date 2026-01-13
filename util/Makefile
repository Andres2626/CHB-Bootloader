# Makefile for tools subdir
TOPDIR 		:= ..
CHB_INC		:= $(TOPDIR)/include
TOP_INC		:= $(TOPDIR)
THIS_INC	:=./
OUT			:= $(TOPDIR)/$(OUT)
OUTDIR  	:= $(OUT)/util
Q			:= @
PROGS		:= $(OUT)/install
TOOLS_INCS	+= -I$(TOP_INC) -I$(THIS_INC) -I$(CHB_INC)
CFLAGS += $(TOOLS_INCS) -DCHB_UTIL

.PHONY: all clean

all_util: $(OUTDIR) $(PROGS)

$(OUT)/install: $(OUTDIR)/install.o
	@echo UTIL $@
	$(Q)$(HOST_CC) -o $@ $^ -largp

$(OUTDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo HOST_CC $<
	$(Q)$(HOST_CC) -c -o $@ $< $(CFLAGS)
	
clean_util:
	rm -rf $(PROGS)
	rm -rf $(OUTDIR)/*

$(OUTDIR):
	mkdir -p $@