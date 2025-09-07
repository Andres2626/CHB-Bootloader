
/*
* loader.S -- begin of stage2
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE
#define LOADER_FILE

#include "CPU.S" /* enter_protmode */

#include <CHB/macros.h>
#include <CHB/stage1/stage1.h>

.file "loader.S"

.code16

SECTION(text)

.global _start 
_start: 
	jmp real_start 
	nop

.org 0x8

params: 
drive_number:
	.byte 0

.org 0x11

real_start: 
	cli /* disable interrupts */

	/* set stack localization in memory */
	mov $0xFFF0, %sp 
	mov %sp, %bp

	/* save the drive number */
	movb %dl, drive_number

	/* load our GDT */
	call load_gdt
	
	/* reload disk system */
	xor %ah, %ah
	int $0x13

	/* enter to protected mode */
	enter_protmode
	.code32

	/* prepare C enviroment -- clean bss section */
	mov $__bss_start, %edi 
	mov $_end, %ecx 
	sub %edi, %ecx 
	xor %al, %al 
	cld 
	rep stosb

    /* save drive number */
	xor %edx, %edx 
	movb drive_number, %dl 
	push %edx
	call entry 
	jmp stop

stop: 
	jmp stop

.code16

FUNCTION(load_gdt)
	lgdt gdt 
	ret

.align 2

/* set GDT entry */
.macro _GDT_ENTRY a, b
.word 0xFFFF, 0 
.byte 0 
.byte \a 
.byte \b, 0 
.endm

/* set GDT descriptor */
.macro _GDT_SET_DESC a, b, c, d
.word \a, \b, \c, \d
.endm

#define _GDT_SIZE gdt - _gdt

/* This is a global descriptor table (GDT) used for change to 
unreal/protected mode*/
_gdt :
/* null descriptor */
_GDT_SET_DESC 0, 0, 0, 0

_GDT_ENTRY 0x9a 0xcf /* 32bit code descriptor */
_GDT_ENTRY 0x92 0xcf /* 32bit data descriptor */
_GDT_ENTRY 0x9a 0x0c /* 16bit code descriptor */
_GDT_ENTRY 0x92 0x0c /* 16bit data descriptor */

GLOBAL(gdt)
.word _GDT_SIZE - 1 
.long _gdt
