CHB_VERSION		:= CHB_0.1


# Enable interface for cross compiling platform
CROSS		:= 
ifneq ($(CROSS_PREFIX), )
	CROSS 	:= $(CROSS_PREFIX)
endif

# OPTIONAL: Configure your compiler dir
COMPDIR		:=
ifneq ($(COMPILER_DIR), )
	COMPDIR	:= $(COMPILER_DIR)
endif

# Use the 64-bit compiler compatibility
# this macro adds -m64 flag in gcc compiler and -melf_i386
USE_X86_64	:= NO
ifeq ($(USE_86-64), )
USE_X86_64	:= YES
endif

# Configure your enviroment
CC			:=$(COMPDIR)$(CROSS_PREFIX)gcc
LD			:=$(COMPDIR)$(CROSS_PREFIX)ld
AS			:=$(CC)
CPP			:=$(COMPDIR)$(CROSS_PREFIX)g++
NM			:=$(COMPDIR)$(CROSS_PREFIX)nm
AR			:=$(COMPDIR)$(CROSS_PREFIX)ar
READELF		:=$(COMPDIR)readelf
OBJCOPY		:=$(COMPDIR)objcopy
OBJDUMP		:=$(COMPDIR)objdump
SH			:=$(COMPDIR)sh
BASH		:=$(COMPDIR)bash
PHTYHON		:=$(COMPDIR)python
MKDIR 		:=$(COMPDIR)mkdir -p
RMDIR		:=$(COMPDIR)rmdir
RM			:=$(COMPDIR)rm -rf
BOCHS		:=$(COMPDIR)bochs -f 

# project directories
INCLUDEDIR	:=Include
SRCDIR		:=Src
SCRIPSDIR	:=Scripts

# Directories to create
STAGE1_OUT_DIR += Out/$(CROSS)/stage1
CREATEDIRS	+= $(STAGE1_OUT_DIR)

# source directories
STAGE1_SRC_DIR += $(SRCDIR)/stage1
STAGE1_INC_DIR += $(INCLUDEDIR)/CHB/stage1

# Output images
STAGE1_IMG	:= $(STAGE1_OUT_DIR)/stage1.IMG
OS_IMG		:= ./os.IMG

# stage1 objs and headers
STAGE1_OBJS	:= $(STAGE1_OUT_DIR)/stage1.o
STAGE1_INCS	:= $(STAGE1_INC_DIR)/stage1.h

# Tools
FLOPPY_RUN	:= ./$(SCRIPSDIR)/floppy_run.sh $(OS_IMG)

# compiler flags
CFLAGS		:= -nodefaultlibs -nostdlib -ffreestanding -mno-red-zone -fpic -I./$(INCLUDEDIR) -I./$(INCLUDEDIR)/CHB

PHONY		:= all clean install run debug
.PHONY: $(PHONY)
all: $(CREATEDIRS) $(OS_IMG)

$(OS_IMG): $(STAGE1_OUT_DIR)/stage1.IMG
	cat $^ > $@

$(STAGE1_OUT_DIR)/stage1.IMG: $(STAGE1_OBJS)
	$(LD) -o $@ $^ -Ttext=7C00 --oformat binary 

$(STAGE1_OUT_DIR)/%.o: $(STAGE1_SRC_DIR)/%.S
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	$(RM) Out/*

install:

$(CREATEDIRS):
	$(MKDIR) $@

run: $(OS_IMG)
	$(SH) $(FLOPPY_RUN)

debug: $(OS_IMG)
	$(BOCHS) $(SCRIPSDIR)/floppy_debug

