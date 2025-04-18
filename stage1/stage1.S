
/*
* stage1.S -- bootsector of CHB
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE
#define STAGE1_FILE

#include <sizes.h>
#include <CHB/macros.h>
#include <CHB/stage1/stage1.h>

/* define an string value */
#define STR(name, str) name ## _msg: \
	.string str
	
/* convert no suffixed string to suffixed string */
#define CONV(name) name ## _msg

/* Log functions */
#define _LOG(func) MSG(CONV(func))

#define LOG(func) _LOG(func); \
				_LOG(newline)

/* use 16-bit assembly mode */		
.code16
SECTION(text)
GLOBAL(_start)
	/* special code for ensure compatibility */
	jmp	afther_bpb
	nop

.org BOOT_BPB_START

	/* This is an space commonly used in some filesystems (FAT, NTFS or HPFS) */
	
.org BOOT_BPB_END

drive_number:
	.byte 0 /* provided by BIOS %dl register */
loader_params:
load_loc:
	.word 0x8000 /* where loader is located (in memory) */
loader_sector_loc:
	.word 2 /* where loader is located (in ABS sectors) */
load_size:
	.word ((LOADER_IMG_SIZE + 511) >> 9) /* loader size (in sectors) */
	
afther_bpb:
	/* 
	  some bogus BIOS doesn't charge boot sector correctly.
	  jump to 0x7c00 instead of 0:7c00 
	*/
	ljmp $0, $real_start
	
real_start: 
	cli /* secure mode. */
	movb %dl, drive_number /* save this for use later */
	push %dx /* save value of %dl */
	
	/* setup stack in 0x7C00 */
	xor	%ax, %ax
	mov	%ax, %ss
	mov %ax, %es
	mov %ax, %ss
	mov	$BOOT_LOC, %sp 
	
	/* restore interrupts */
	sti
	_LOG(welcome)
	call enable_a20 /* try enable A20 with the BIOS */
	_LOG(loading)
	
	/* push two sector in 0x8000 */
	xor	%dx, %dx
	movw loader_sector_loc, %bx
	movw load_size, %cx
	mov load_loc, %edi
	call load_floppy
	call sucsess_load
	cli /* secure mode */
	
	/* setup stack loader (stage2) */
	mov	$0x8000, %ax
	mov	%ax, %ss
	mov %ax, %es
	mov %ax, %ss
	pop %dx
	
	/* jmp to loader program */
	ljmp $0, $0x8000
	jmp stop

STR(welcome, "CHB ")
STR(loading, "Loading ")
STR(sucsess, "YES!")

/* print string with INT10,0x0E */
_print:
	pusha
1:
	lodsb
	or %al, %al
	jz 2f
	mov	$0x0e, %ah
	mov	$0x0, %bx
	int	$0x10
	jmp	1b
2:
	popa
	ret

/* ensure a20 is enabled */
enable_a20:
	/* save registers into stack */
	pusha
	
	/* is A20 supported? */
	mov $0x2403, %ax 
	int $0x15
	jb a20_error
	cmp $0, %ah
	jnz a20_error
	
	/* get A20 status */
	mov $0x2402, %ax 
	int $0x15
	jb a20_error
	cmp $0, %ah
	jnz a20_error
	
	/* is A20 enabled? */
	cmp $1, %al
	jz 1f
	
	/* enable A20 from BIOS */
	mov $0x2401, %ax 
	int $0x15
	jb a20_error
	cmp $0, %ah
	jnz a20_error
1:
	popa
	ret

sucsess_load:
	LOG(sucsess)
	ret
a20_error:
	_LOG(mm_a20_error)
	jmp general_error
read_error:
	_LOG(dev_r_error)
	jmp general_error
general_error:
	LOG(general_err)
boot_error:
	int $0x18
stop:
	jmp stop


STR(dev_r_error, "READ ")
STR(mm_a20_error, "A20 ")
STR(general_err, "ERROR.")
STR(newline, "\r\n")

/* simple floppy controller TODO: rework this! */
load_retry:
	.byte 3
load_floppy:
	push %bx # number of sector
	push %cx # sectors to read
load_floppy_loop:
	mov	$0x13, %al # read only 18 sectors because floppy geometry
				   # TODO: get floppy geometry.
	sub	%bl, %al
	xor	%ah, %ah
	cmp	%cx, %ax
	jl 1f
	mov	%cx, %ax
1:
	mov %bx, %cx
	mov $BOOT_DISK_BUFFER, %bx # buffer address
	mov $0x2, %ah # function 2
	push %ax
	int $0x13 # read sectors via INT13,2
	pop %ax
	jnc 2f # sucsess call
	decb load_retry # substract 1 to read retry number
	jz read_error # fail call
	xor %ah, %ah # function 0
	int $0x13 # reset floppy controller via INT13,0
	jmp load_floppy_loop
2:
	movb $3, load_retry # reload retry number
	mov $BOOT_DISK_BUFFER, %esi # set the buffer
	xor %ecx, %ecx
	mov %al, %cl
	shl $0x7, %cx
	rep movsd # mov %ds:%esi to %es:%edi
	pop %cx
	pop %bx
	xor $0x1, %dh # read next head
	jnz 3f
	inc %bh # read next cylinder
3:
	mov $0x1, %bl # set sector in 1
	xor %ah, %ah
	sub %ax, %cx
	jg load_floppy # read left sectors
	ret

/* MBR */
.org MBR_START

/* for make an bootable image the code must have following code. */
.org BOOT_PART_SIZE
.word BOOT_MAGIC /* 0xaa55 */