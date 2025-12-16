TOPDIR 		= ../../
INCFLAGS 	:= -I$(TOPDIR) -I./ -I$(TOPDIR)/include
LDFLAGS 	+= $(TARGET_LDFLAGS)
CFLAGS 		+= -FPIC $(TARGET_CFLAGS) $(INCFLAGS)
OUT			:= $(TOPDIR)/$(OUT)
OUTDIRS  	:= $(OUT)/lib/libc
LIB 		:= libchb_c.a
LIBC    	:= $(addprefix $(OUT)/, $(LIB))
Q			:= @

.PHONY: all_src_loader clean_src_loader

LIB_SRCS := 
LIB_SRCSC:= stdio/printf.c \
			stdio/putchar.c \
			stdio/puts.c \
			stdio/sprintf.c \
			stdio/vsprintf.c \
			stdlib/atoi.c \
			stdlib/itoa.c \
            stdlib/div.c \
			stdlib/itoa64.c \
			stdlib/utoa64.c \
			string/memcmp.c \
			string/memset.c \
			string/memcpy.c \
			string/strchr.c \
			string/strlen.c \
			string/strncmp.c \
			string/strpbrk.c \
			string/strspn.c \
			string/strstr.c \
			string/strtok.c \
			string/strerror.c \
			ctype/upper.c \
			ctype/lower.c \
			
LIB_OBJS := $(patsubst %.S, $(OUTDIRS)/%.o, $(LIB_SRCS)) \
			$(patsubst %.c, $(OUTDIRS)/%.o, $(LIB_SRCSC))

all_lib/libc: $(OUTDIRS) $(LIBC)

$(LIBC): $(LIB_OBJS)
	@echo LIB $@
	$(Q)$(TARGET_AR) rcs $@ $^

$(OUTDIRS)/%.o: %.S
	$(Q)mkdir -p $(@D)
	@echo AS $<
	$(Q)$(TARGET_CC) -c -o $@ $< $(CFLAGS)

$(OUTDIRS)/%.o: %.c
	$(Q)mkdir -p $(@D)
	@echo CC $<
	$(Q)$(TARGET_CC) -c -o $@ $< $(CFLAGS)

clean_lib/libc:
	@echo "Clearing LIBC"
	rm -rf $(LIBC)
	rm -rf $(OUTDIRS)/*
	
$(OUTDIRS):
	$(Q)mkdir -p $@