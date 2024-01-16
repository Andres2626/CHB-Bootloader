TOPDIR = ../..

.PHONY: all_src_boot clean_src_boot

OUT			:= $(TOPDIR)/$(OUT)
OUTDIRS  	:= $(OUT)/stage1
PROG 		:= stage1.IMG
PROGRAMS    := $(addprefix $(OUT)/, $(PROG))

LDFLAGS += $(TARGET_LDFLAGS) -Ttext=0x7C00
CFLAGS += $(TARGET_CFLAGS) -I$(TOPDIR) -I./ -I$(TOPDIR)/include -I$(OUT)

ST1_SRCS := stage1.S
ST1_OBJS := $(patsubst %.S, $(OUTDIRS)/%.o, $(ST1_SRCS))

INCFLAGS 	:= -I$(TOPDIR) -I./ -I$(TOPDIR)/include

all_src_stage1: $(OUTDIRS) $(PROGRAMS)

$(OUT)/stage1.IMG: $(OUTDIRS)/stage1.build
	$(TARGET_OBJCOPY) -O binary $< $@

$(OUTDIRS)/stage1.build: $(ST1_OBJS)
	$(TARGET_LD) $< -o $@ $(LDFLAGS)

$(OUTDIRS)/%.o: %.S
	@mkdir -p $(@D)
	$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_src_stage1:
	@echo "Clearing stage1 program"
	rm -rf $(PROGRAMS)
	rm -rf $(OUTDIRS)/stage1.build
	rm -rf $(OUTDIRS)/*.o
	rmdir $(OUTDIRS)
	
$(OUTDIRS):
	mkdir -p $@