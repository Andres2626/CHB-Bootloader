
#define ASM_FILE
#define LOADER_FILE

#include "CPU.S" /* enter_protmode */

#include <CHB/stage1/stage1.h>
#include <CHB/macros.h>

/*
 * TODO:
 * - Add code for call C code (cmain function)
 * - Add support for Hard disk and CDROMS
 * - Implement full suport for extended disk read mode (INT13,42)
 */


.file "loader.S"

.code16

SECTION(text)

.global _start
_start:
	jmp real_start
	
.org 0x8

params:
drive_number:
	.byte 0
kernel_loc:
	.long 0x100000 /* localization of kernel in memory */
kernel_sector_loc:
	.word 5 /* TODO: Get this data dinamically */
kernel_size:
	.word 5

.org 0x11

real_start:
	cli /* disable interrupts */

	/* save the drive number */
	movb %dl, drive_number

	/* load our GDT */
	call load_gdt
	
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
	mov (params), %edx
	push %edx
	
	/* save kernel parameters */
	xor %edx, %edx
	mov $0x10 - params, %edx
	push %edx
	
	call entry
	jmp stop

.code16

/* print string with INT10,0x0E */
_print:
	pusha
1:
	lodsb
	or %al, %al
	jz 2f
	mov	$0x0e, %ah
	xor	%bx, %bx
	int	$0x10
	jmp	1b
2:
	popa
	ret
	
boot_error:
	int $0x18
stop:
	jmp stop

FUNCTION(load_gdt)
	lgdt gdt
	ret
	
/* This is a global descriptor table (GDT) used for change to unreal/protected mode*/
.p2align 2 /* force alignement */
_gdt:
	/* null descriptor */
	.word 0, 0, 0, 0
	
	/* code32 desc */
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0x9a
	.byte 0xcf
	.byte 0

	/* data32 desc */
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0x92
	.byte 0xcf
	.byte 0	
	
	/* code16 desc */
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0x9a
	.byte 0x0c
	.byte 0
	
	/* data16 desc */
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0x92
	.byte 0x0c
	.byte 0
	
GLOBAL(gdt)
	.word gdt - _gdt
	.long _gdt
