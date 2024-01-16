
TOPDIR = ../..

.PHONY: all_src_loader clean_src_loader

LDFLAGS += $(TARGET_LDFLAGS) -Ttext=0x8000
CFLAGS += $(TARGET_CFLAGS) -I$(TOPDIR) -I./ -I$(TOPDIR)/include

OUT			:= $(TOPDIR)/$(OUT)
OUTDIRS  	:= $(OUT)/loader
PROG 		:= loader.IMG
PROGRAMS    := $(addprefix $(OUT)/, $(PROG))

LOD_SRCS := loader.S
LOD_OBJS := $(patsubst %.S, $(OUTDIRS)/%.o, $(LOD_SRCS))

INCFLAGS 	:= -I$(TOPDIR) -I./ -I$(TOPDIR)/include

all_src_loader: $(OUTDIRS) $(PROGRAMS)

$(OUT)/loader.IMG: $(OUTDIRS)/loader.build
	$(TARGET_OBJCOPY) -O binary $< $@
	$(TOPDIR)/scripts/size-test.sh $(OUT)/sizes.h $@ 1

$(OUTDIRS)/loader.build: $(LOD_OBJS)
	$(TARGET_LD) $< -o $@ $(LDFLAGS)

$(OUTDIRS)/%.o: %.S
	@mkdir -p $(@D)
	$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_src_loader:
	@echo "Clearing loader program"
	rm -rf $(PROGRAMS)
	rm -rf $(OUTDIRS)/loader.build
	rm -rf $(OUTDIRS)/*.o
	rmdir $(OUTDIRS)
	
$(OUTDIRS):
	mkdir -p $@