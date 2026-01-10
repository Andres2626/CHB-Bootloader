
this_dir	:= .
top			:= $(this_dir)

PHONY 	+= 	all \
			clean \
			recomp \
			help \
			supported_target_prefixes
			
.PHONY: $(PHONY)

all: begin_build

help:
	@echo '*****   CHB General help   *****'
	@echo '  make TARGET_PREFIX=[target] HOST_PREFIX=[host]	- General build'
	@echo '  make V=1						- View debug build INFO'
	@echo '  make all						- Build CHB'
	@echo '  make clean						- Clean output files of CHB'
	@echo '  make recomp						- Recompile CHB'
	@echo '  make supported_target_prefixes			- View an list of supported targets'
	@echo '  make help						- Print THIS'

supported_target_prefixes:
	@echo '*****	CHB Supported targets	*****'
	@echo 'This is a list of CHB Supported target prefixes'
	@echo '	* i[x]86-elf -- Build for i386, i686 platforms'
	@echo '	* x86_64-elf -- Build for x86_64 elf platforms'
	@echo
	@echo ' for use target platform try this 'make TARGET_PREFIX=[your-target]-''
	@echo 'TODO: Add more platforms in future!'

# detect if path have spaces
ifneq ($(words $(top)),1)
    $(error BUILD ERROR: The main directory cannot contain spaces)
endif

# Initial configuration
boot		:= boot
docs		:= docs
stage1		:= stage1
include		:= include
lib			:= lib
shared		:= shared
utils		:= utils
build		:= build
fs			:= fs

debug_build := $(build)/debug

ifeq ($(V), )
	Q		:= @
endif

empty		:=
host		:= $(empty)
ifneq ($(HOST_PREFIX), )
	host 	.= $(HOST_PREFIX)
endif

target		:= $(empty)
ifneq ($(TARGET_PREFIX), )
	target 	:= $(TARGET_PREFIX)
endif

# setup GCC and binutils target and host programs
TARGET_GCC	:= $(target)gcc
TARGET_LD	:= $(target)ld
TARGET_AR	:= $(target)ar
TARGET_NM	:= $(target)nm
TARGET_OBJDUMP	:= $(target)objdump
TARGET_OBJCOPY	:= $(target)objcopy
TARGET_STRIP	:= $(target)strip
TARGET_READELF	:= $(target)strip
HOST_GCC	:= $(host)gcc
HOST_LD		:= $(host)ld
HOST_AR		:= $(host)ar
HOST_NM		:= $(host)nm
HOST_OBJDUMP	:= $(host)objdump
HOST_OBJCOPY	:= $(host)objcopy
HOST_STRIP		:= $(host)strip
HOST_READELF	:= $(host)strip

TARGET_IFLAGS := -I./ -I$(this_dir)/include
TARGET_CFLAGS += $(TARGET_IFLAGS) -ffreestanding -nostdlib -nodefaultlibs -mno-red-zone -FPIC -m32
TARGET_SFLAGS +=
TARGET_LDFLAGS +=
HOST_IFLAGS := -I$(this_dir)/include/fs -I$(this_dir)/include/shared
HOST_CFLAGS += $(HOST_IFLAGS) -DUSE_HOST
HOST_SFLAGS +=
HOST_LDFLAGS +=

# build libc
libc_srcs	:= lib/libc
libc_obj	:= $(build)/$(libc_srcs)
LIBC 		:= $(build)/libc.a
libc_string_srcs	:= $(shell find $(libc_srcs)/string -iname *.c)
libc_stdio_srcs		:= $(shell find $(libc_srcs)/stdio -iname *.c)
libc_stdlib_srcs	:= $(shell find $(libc_srcs)/stdlib -iname *.c)
libc_ctype_srcs		:= $(shell find $(libc_srcs)/ctype -iname *.c)
LIBC_SRCS:= $(libc_string_srcs) $(libc_stdio_srcs) $(libc_stdlib_srcs) $(libc_ctype_srcs)
LIBC_OBJS:= $(patsubst %.c, $(build)/%.o, $(LIBC_SRCS))
LIBC_DEPS:= $(patsubst %.o, %.d, $(LIBC_OBJS))

$(LIBC): $(LIBC_OBJS)
	$(Q)$(TARGET_AR) rcs $@ $^

# build boot program
boot_src	:= $(boot)
boot_obj	:= $(build)/$(boot_src)
BOOT		:= $(build)/boot.IMG
BOOT_SRCS	:= $(shell find $(boot_src) -iname *.S)
BOOT_OBJS 	:= $(patsubst %.S, $(build)/%.o, $(BOOT_SRCS))
BOOT_DEPS	:= $(patsubst %.o, %.d, $(BOOT_OBJS))
BOOT_LDFLAGS := $(TARGET_LDFLAGS) -T"$(boot_src)/bootsector.ld"

$(BOOT): $(build)/stage1_bootsector.elf
	$(Q)$(TARGET_OBJCOPY) -O binary $< $@
	$(Q)$(TARGET_OBJDUMP) -Maddr16,data16 -mi8086 -d $< > $(build)/debug/stage1_bootsector.elf.16-bit.dasm
	
$(build)/stage1_bootsector.elf: $(BOOT_OBJS)
	$(Q)$(TARGET_LD) $< -o $@ $(BOOT_LDFLAGS)
	$(Q)$(TARGET_NM) $@ > $(build)/debug/stage1_bootsector.elf.sym

# build stage1 program
stage1_src	:= $(stage1)
stage1_obj	:= $(build)/stage1
STAGE1		:= $(build)/stage1.IMG
STAGE1_CCS	:= $(stage1_src)/stage1.c $(shared)/disk.c $(stage1_src)/memory.c
STAGE1_S	:= $(stage1_src)/gdt.S $(shared)/asm.S
STAGE1_OBJSC:= $(patsubst %.c, $(build)/%.o, $(STAGE1_CCS))
STAGE1_OBJSS:= $(patsubst %.S, $(build)/%.o, $(STAGE1_S))
STAGE1_OBJS := $(STAGE1_OBJSC) $(STAGE1_OBJSS)
STAGE1_DEPS	:= $(patsubst %.o, %.d, $(STAGE1_OBJS))
STAGE1_LDFLAGS	:= $(TARGET_LDFLAGS) -T"$(stage1_src)/stage1.ld" $(LIBC)

$(STAGE1): $(build)/stage1.elf
	$(Q)$(TARGET_OBJCOPY) -O binary $< $@
	$(Q)$(TARGET_OBJDUMP) -Maddr32,data32 -mi8086 -d $< > $(build)/debug/stage1.elf.32-bit.dasm
	
$(build)/stage1.elf: $(STAGE1_OBJS)
	$(info $(STAGE1_SRCS))
	$(Q)$(TARGET_LD) $^ -o $@ $(STAGE1_LDFLAGS)
	$(Q)$(TARGET_NM) $@ > $(build)/debug/stage1.sym

# build utils program
utils_src	:= $(utils)
utils_obj 	:= $(build)/utils

COPYCHB		:= $(build)/copychb
COPYCHB_CS	:= $(utils_src)/copychb.c
COPYCHB_OBJS	:= $(patsubst %.c, $(build)/%.o, $(COPYCHB_CS))
COPYCHB_DEPS	:= $(patsubst %.o, %.d, $(COPYCHB_OBJS))

UTILS_PROGS	:= $(COPYCHB)
UTILS_DEPS	:= $(COPYCHB_DEPS)

$(build)/copychb: $(COPYCHB_OBJS)
	@mkdir -p $(@D)
	$(Q)$(HOST_GCC) -o $@ $^ $(HOST_CFLAGS)

-include $(UTILS_DEPS)
-include $(LIBC_DEPS)
-include $(BOOT_DEPS)
-include $(STAGE1_DEPS)

$(build)/%.o: %.c
	@mkdir -p $(@D)
	$(Q)$(TARGET_GCC) -MMD -c -o $@ $< $(TARGET_CFLAGS)

$(build)/%.o: %.S
	@mkdir -p $(@D)
	$(Q)$(TARGET_GCC) -MMD -c -o $@ $< $(TARGET_CFLAGS)

$(build)/utils/%.o: utils/%.c
	@mkdir -p $(@D)
	$(Q)$(HOST_GCC) -MMD -c -o $@ $< $(HOST_CFLAGS)

clean:
	rm -rf $(build)/*
	rmdir $(build)

begin_build: $(build)/debug $(build) boot filesystem stage1 utils timestamp
boot: $(BOOT)
stage1: $(LIBC) $(STAGE1)
utils: $(UTILS_PROGS)
filesystem: $(FS_OBJS)

TIME= $(shell date --utc +%FT%TZ)
timestamp:
	@echo " " > $(build)/timestamp.h
	@echo "/* This is autogenerated by makefile used for debug purposes  */ " >> $(build)/timestamp.h
	@echo "#define BUILD_TIMESTAMP $(TIME)" >> $(build)/timestamp.h

$(build):
	mkdir -p $@

$(build)/debug:
	mkdir -p $@
	
recomp: clean all