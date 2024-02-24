
TOPDIR = ../../..

.PHONY: all_src_loader clean_src_loader

LDFLAGS += $(TARGET_LDFLAGS)
CFLAGS += $(TARGET_CFLAGS) -ffreestanding -fno-exceptions -FPIC -mno-red-zone -nodefaultlibs -nostdlib -I$(TOPDIR) -I./ -I$(TOPDIR)/include

OUT			:= $(TOPDIR)/$(OUT)
OUTDIRS  	:= $(OUT)/lib/libc
LIB 		:= libchbc.a
LIBC    	:= $(addprefix $(OUT)/, $(LIB))

LIB_SRCS := 
LIB_SRCSC:= stdio/getchar.c \
			stdio/printf.c \
			stdio/putchar.c \
			stdio/puts.c \
			stdio/sprintf.c \
			stdio/vprintf.c \
			stdio/vsprintf.c \
			stdlib/itoa.c \
			string/memcmp.c \
			string/memcpy.c \
			string/strchr.c \
			string/strlen.c \
			string/strncmp.c \
			string/strpbrk.c \
			string/strspn.c \
			string/strstr.c \
			string/strtok.c
			
LIB_OBJS := $(patsubst %.S, $(OUTDIRS)/%.o, $(LIB_SRCS)) \
			$(patsubst %.c, $(OUTDIRS)/%.o, $(LIB_SRCSC))

INCFLAGS 	:= -I$(TOPDIR) -I./ -I$(TOPDIR)/include

all_src_lib/libc: $(OUTDIRS) $(LIBC)

$(LIBC): $(LIB_OBJS)
	$(TARGET_AR) rcs $@ $^

$(OUTDIRS)/%.o: %.S
	@mkdir -p $(@D)
	$(TARGET_CC) -c -o $@ $< $(CFLAGS)

$(OUTDIRS)/%.o: %.c
	@mkdir -p $(@D)
	$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_src_lib/libc:
	@echo "Clearing LIBC"
	rm -rf $(LIBC)
	rm -rf $(OUTDIRS)/*
	
$(OUTDIRS):
	mkdir -p $@