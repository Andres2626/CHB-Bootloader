/*
* loader.S -- begin of stage2
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE
#define LOADER_FILE

#include "config.h"

#include "arch/x86/asm/CPU.S"
#include "arch/x86/stack.h"
#include "arch/x86/macros.h"

#include "stage1/stage1.h"

.file "loader.S"

.code16

.text

FUNCTION(_start)
start:
	jmp loader_start 
	nop

.org BOOT_BPB_START

/* 
 * This is CHB header used by C part for detect some reported info
 * Offset  Size     Description
 * 0x0     0x4      CHB magic number, must be '0x93A76FD8' for sucessful load.
 * 0x4     0x4      Header size. (0x5C for 0.06)
 * 0x8     0x1      Reported disk number (BIOS)
 * 0x9     0x3      Version number (byte 0: Major, byte 1: Minor, byte 2: Revision)
 * 0xC     0x10     Version string.
 * 0x1C    0x20     First option that CHB can boot from FS.
 * 0x3C    0x20     Second boot option TODO: Not implemented.
 * 
 * For CHB to detect the header, it must be set to 0x8000 + 0x4. 
 */
VARIABLE(loader_hdr)
VARIABLE(magic_number)
    .long 0x93A76FD8
VARIABLE(header_size)
    .long loader_hdr_end - loader_hdr
VARIABLE(drive_number)
	.byte 0
VARIABLE(loader_version)
    .byte VERSION_MAJOR, VERSION_MINOR, VERSION_REV
VARIABLE(loader_string)
    .string CHB_VERSION_STRING
    .space 16 - (. - loader_string)
VARIABLE(loader_first_boot)
    .string CHB_FIRST_BOOT_OPTION
    .space 32 - (. - loader_first_boot)
VARIABLE(loader_second_boot)
    .byte 0xff
    .space 32 - (. - loader_second_boot)
VARIABLE(loader_hdr_end)

/* reserve for future versions */

.space 0x100 - (. - _start)

loader_start:
    cli
    /* set data registers */
    xor %ax, %ax

    /* set all data registers */	
	mov %ax, %ds
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss

    /* set stack localization in memory */
	movw $0xF000, %sp 
    movw %sp, %bp

	/* save the drive number */
	movb %dl, drive_number /* reported by stage1 */

    /* restore interrupts */
	sti
	
	/* reload disk system */
	xor %ah, %ah
	int $0x13

    cli
	
	/* try enable A20 */
	call enable_a20
	cmp $0, %al
	je a20_error

	/* enter to protected mode */
	enter_protmode

	/* prepare C enviroment -- clean bss section */
	mov $__bss_start, %edi 
	mov $_end, %ecx 
	sub %edi, %ecx 
	xor %al, %al 
	cld 
	rep stosb
	
	xor %edx, %edx
    
    /* load header */
	leal loader_hdr, %edx
	
	/* put drive_number to C function */
	push %edx
	
	/* init C function */
	call init_loader
	
	pop %edx
	
	/* stop program */
	jmp stop

/* Infinite loop */
stop: 
	jmp stop

VARIABLE(a20_error_str)
    .string "A20 Enable error"
VARIABLE(newline)
	.string "\r\n"

FUNCTION(a20_error)
    MSG(a20_error_str)
	MSG(newline)
	jmp stop

.code16
FUNCTION(_print)
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

.code16
/*
 * Enable A20 functions
 * 1. Test if A20 is enabled
 * 2. Enable A20 (fast)
 * 3. Enable from BIOS
 */
FUNCTION(enable_a20)
   /* test if A20 is enabled */
   in $0x92, %al
   test $0x02, %al
   jnz 1f
   
   /* enable a20 fast */
   or $0x02, %al
   out %al, $0x92
   
   /* test 2 */
   in $0x92, %al
   test $0x02, %al
   jz 2f /* error */
1:
   xor %eax, %eax
   mov $1, %al
   jmp 4f
2:
   /* second try - enable from BIOS */
   
   /* is A20 supported? */
	mov $0x2403, %ax 
	int $0x15
	jb 3f
	cmp $0, %ah
	jnz 3f
	
	/* get A20 status */
	mov $0x2402, %ax 
	int $0x15
	jb 3f
	cmp $0, %ah
	jnz 3f
	
	/* is A20 enabled? */
	cmp $1, %al
	jz 1f
	
	/* enable A20 from BIOS */
	mov $0x2401, %ax 
	int $0x15
	jb 3f
	cmp $0, %ah
	jnz 3f
	
	/* sucess */
	jmp 1f
3:
    mov $0, %al
4:
    ret

/*
 * Change from protected mode to real mode
 * NOTE: This function change the value of '%eax'
 */
.code32
FUNCTION(switch_to_real_mode) 
    /* disable interrupts */
	cli
	
	/* load 'GDT' */
	lgdt gdt
	
	/* setup new data segments */
	mov $PROT_16_DATA_SEG, %ax 
	
	/* set data registers */
	mov %ax, %ds 
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss
	
	/* enter to 16-bit protected mode */
	ljmp $PROT_16_CODE_SEG, $1f

/* 16-bit protected mode */
1:
.code16
	/* disable CR0 protection bit */
	movl %cr0, %eax 
	andl $CR0_DISABLE, %eax 
	movl %eax, %cr0

	/* jump to real mode */
	ljmp $REAL_16_CODE_SEG, $2f

/* 16-bit real mode */
2:
	/* setup data registers */
	xor %ax, %ax 
	
	/* set all data registers to 0 */
	mov %ax, %ds
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss
	
	/* restore interrupts */
	sti
	
	/* return to macro */
	ret
	
.code16
FUNCTION(switch_to_prot_mode)
    /* disable interrupts */
	cli
    
    /* load GDT */
	lgdt gdt
	
	/* enable CR0 protection bit */
	movl %cr0, %eax 
	orl $CR0_ENABLE, %eax
	movl %eax, %cr0

	/* enter to 32-bit protected mode */
	ljmp $PROT_32_CODE_SEG, $1f

.code32 
1:
	/* setup protected mode data registers*/
	mov $PROT_32_DATA_SEG, %ax 
	mov %ax, %ds
	mov %ax, %es 
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss
	
	xor %eax, %eax
	
	/* return to macro */
	ret

/* force 2-bit alignment */
.align 2

#define GDT_SIZE gdt - _gdt

/* This is a GDT used for switch beetween CPU modes */
_gdt:
	/* null descriptor */
	GDT_ENTRY 0, 0, 0, 0
	
	/* 'magic' descriptors */
	GDT_ENTRY 0, 0xFFFFF, 0x9a, 0xC /* 32-bit CD */
	GDT_ENTRY 0, 0xFFFFF, 0x92, 0xC /* 32-bit DS */
	GDT_ENTRY 0, 0xFFFF, 0x9E, 0x0/* 16-bit CD */
	GDT_ENTRY 0, 0xFFFF, 0x92, 0x0 /* 16-bit DS */


/* used for load gdt via 'lgdt' instruction */
GLOBAL(gdt)
	.word GDT_SIZE - 1 
	.long _gdt