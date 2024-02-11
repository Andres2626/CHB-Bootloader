
TOPDIR = ../..

.PHONY: all_src_loader clean_src_loader

LDFLAGS += $(TARGET_LDFLAGS) -T"loader.LD"
CFLAGS += $(TARGET_CFLAGS) -ffreestanding -fno-exceptions -mno-red-zone -nodefaultlibs -nostdlib -I$(TOPDIR) -I./ -I$(TOPDIR)/include

OUT			:= $(TOPDIR)/$(OUT)
OUTDIRS  	:= $(OUT)/loader
PROG 		:= loader.IMG
PROGRAMS    := $(addprefix $(OUT)/, $(PROG))

LOD_SRCS := loader.S video.S port.S
LOD_SRCSC:= entry.c
LOD_OBJS := $(patsubst %.S, $(OUTDIRS)/%.o, $(LOD_SRCS)) \
			$(patsubst %.c, $(OUTDIRS)/%.o, $(LOD_SRCSC))

INCFLAGS 	:= -I$(TOPDIR) -I./ -I$(TOPDIR)/include

all_src_loader: $(OUTDIRS) $(PROGRAMS)

$(OUT)/loader.IMG: $(OUTDIRS)/loader.build
	$(TARGET_OBJCOPY) -O binary $< $@
	$(TOPDIR)/scripts/size-test.sh $(OUT)/sizes.h $@ 1

$(OUTDIRS)/loader.build: $(LOD_OBJS)
	$(TARGET_LD) $^ -o $@ $(LDFLAGS)

$(OUTDIRS)/%.o: %.S
	@mkdir -p $(@D)
	$(TARGET_CC) -c -o $@ $< $(CFLAGS)

$(OUTDIRS)/%.o: %.c
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