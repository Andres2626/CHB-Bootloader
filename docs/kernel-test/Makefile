
SFILES:= entry.S
CFILES:= kernel.c

OUTDIRS:= ./build
OUTPUT_IMAGES:= $(OUTDIRS)/kernel.IMG $(OUTDIRS)/final.IMG 

CHBIMG := ../../build/CHB.IMG

LDFLAGS += $(TARGET_LDFLAGS) -Ttext=0x100000
CFLAGS += $(TARGET_CFLAGS) -I$(TOPDIR) -I./ -I$(TOPDIR)/include -I$(OUT)

all_docs/kernel-test: $(OUTDIRS) $(CHBIMG) $(OUTPUT_IMAGES)

$(OUTDIRS)/%.o: %.c
	@mkdir -p $(@D)
	$(TARGET_CC) -c $< -o $@ $(CFLAGS)

$(OUTDIRS)/%.o: %.S
	@mkdir -p $(@D)
	$(TARGET_CC) -c $< -o $@ $(CFLAGS)

$(OUTDIRS)/kernel.IMG: $(OUTDIRS)/entry.o $(OUTDIRS)/kernel.o
	$(TARGET_LD) $(LDFLAGS) $^ -o $(OUTDIRS)/kernel.elf -melf_i386
	$(TARGET_OBJCOPY) -O binary $(OUTDIRS)/kernel.elf $@

$(OUTDIRS)/final.IMG:
	dd count=120 if=/dev/zero of=$@ bs=512
	dd if=$(CHBIMG) of=$@ seek=0 conv=notrunc
	dd if=$(OUTDIRS)/kernel.IMG of=$@ seek=40 conv=notrunc

clean_docs/kernel-test:
	rm -rf $(OUTDIRS)/*.IMG
	rm -rf $(OUTDIRS)/*.o
	rm -rf $(OUTDIRS)/*.elf
	rmdir $(OUTDIRS)

$(OUTDIRS):
	mkdir -p $@