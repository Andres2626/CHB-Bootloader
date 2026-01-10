
# include stage1 configuration
-include ./Makefile.conf

CFLAGS		:= $(INCFLAGS) -nostdlib -nodefaultlibs -fno-rtti -mno-red-zone -FPIC -m32
LFLAGS		:= -T"bootsector.ld"

BOOTSEC		:= $(OBJDIR)/bootsector.elf

BOOTSEC_IMG := $(OBJDIR)/bootsector.IMG

.PHONY : all

$(BOOTSEC_IMG): $(BOOTSEC)
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -mi8086 -d $< > $@.16-bit.lst

$(BOOTSEC): $(BOOTSEC_OBJS)
	$(LD) $< -o $@ $(LFLAGS)

$(OBJDIR)/%.o: %.S
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $< $(CFLAGS)
	
$(OBJDIR)/%.o: %.c %.h
	@mkdir -p $(@D)
	$(GCC) -c -o $@ $< $(CFLAGS)

all: $(BOOTSEC_IMG)

