
SFILES:= entry.S
CFILES:= kernel.c

OUTPUT_IMAGES:= final.IMG kernel.IMG

CHBIMG := ../../build/CHB.IMG

LDFLAGS += $(TARGET_LDFLAGS) -Ttext=0x7C00
CFLAGS += $(TARGET_CFLAGS) -I$(TOPDIR) -I./ -I$(TOPDIR)/include -I$(OUT)

all_docs/kernel-test: $(CHBIMG) kernel.IMG final.IMG

%.o: %.c
	$(TARGET_CC) -c $< -o $@ $(CFLAGS)

%.o: %.S
	$(TARGET_CC) -c $< -o $@ $(CFLAGS)

kernel.IMG: entry.o kernel.o
	$(TARGET_LD) -Ttext=0x100000 $^ -o kernel.ld -melf_i386
	$(TARGET_OBJCOPY) -O binary kernel.ld $@

final.IMG:
	dd count=10 if=/dev/zero of=$@ bs=512
	dd if=$(CHBIMG) of=$@ seek=0 conv=notrunc
	dd if=kernel.IMG of=$@ seek=5 conv=notrunc

clean_docs/kernel-test:
	rm -rf ./*.IMG
	rm -rf ./*.o
