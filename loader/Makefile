# makefile for loader program (stage2)
# Makefile properties
TOPDIR 		:= ../
CHB_INC		:= $(TOPDIR)/include
TOP_INC		:= $(TOPDIR)
THIS_INC	:=./
STAGE2_LOC	:= 0x8000
STAGE2_INCS	+= -I$(TOP_INC) -I$(THIS_INC) -I$(CHB_INC) 
SIZE		:= $(TOPDIR)/scripts/size-test.sh 
OUT			:= $(TOPDIR)/$(OUT)
OUTDIR  	:= $(OUT)/loader
PROG 		:= loader.IMG
PROGRAMS    := $(addprefix $(OUT)/, $(PROG))
SIZE_FILE	:= $(OUT)/sizes.h
Q			:= @

# assembly files
SFILES := loader.S \
		  int10_handler.S \
		  int13_handler.S \
		  port.S
	
# .c files
CFILES:= entry.c \
		 video.c \
		 disk.c \
		 fat.c

# .o files
OFILES := $(patsubst %.S, $(OUTDIR)/%.o, $(SFILES)) \
	      $(patsubst %.c, $(OUTDIR)/%.o, $(CFILES))

LDFLAGS += $(TARGET_LDFLAGS) -L"$(OUT)" -Ttext=$(STAGE2_LOC) -lchb_c
CFLAGS += $(TARGET_CFLAGS) $(STAGE2_INCS)

.PHONY: all_src_loader clean_src_loader

all_loader: $(OUTDIR) $(SIZE) $(SIZE_FILE)

$(SIZE):
	$(error $(SIZE) script not found)

$(SIZE_FILE): $(OUT)/loader.IMG
	@echo SIZE $< 
	$(Q)$(SIZE) $@ $< 1

# binary file
$(OUT)/loader.IMG: $(OUTDIR)/loader.pre
	@echo OCPY $@
	$(Q)$(TARGET_OBJCOPY) -O binary $< $@

# linkable file
$(OUTDIR)/loader.pre: $(OFILES)
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

clean_loader:
	rm -rf $(OFILES)
	rm -rf $(SIZE_FILE)
	rm -rf $(PROGRAMS)
	rm -rf $(OUTDIR)/loader.pre
	rmdir $(OUTDIR)

$(OUTDIR):
	$(Q)mkdir -p $@