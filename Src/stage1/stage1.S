
/* this tells to the compiler 'use 16-bit elf outputs' */
.code16

#include "stage1/stage1.h"

.global _start
_start:
	/* Make a stage1 compatible with some filesystems ej. FAT*/
	jmp entry
	nop

. = _start + BPB_START

	/*
	* This space is a BIOS parameter block in some filesystems DO NO REMOVE THIS!
	*/

. = _start + BPB_END
	
entry:
	/* disable BIOS interrupts */
	cli

	/* 
	* Some bogus BIOS doesn't jump to the boot sector code correcly for solve this,
	* the code jumps to 07C0:0000 instead 0000:7C00
	*/
	ljmp $0, $real_start
	jmp stop_boot
	
real_start:
	/* setup stack! */
	xor %ax, %ax
	mov %ax, %ds
	mov %ax, %ss
	
	/* set the localization of stack */
	mov $BOOT_STACK_LOC, %sp
	
	/* restore BIOS interrupts */
	sti
	
	MSG(welcome_message)
	
	jmp stop_boot

/* This code tells to the firmware 'continue booting'*/
continue_boot:
	int $0x18

/* 
* In some hardware the hlt instruction deadlocks the CPU making CTRL+ALT+DEL not work, for solve this
* only jumps to respective stop-instruction
*/
stop_boot:
	jmp stop_boot

/* 
* Print to the display via INT13, 0x0e 
* params:
* ds:si pointer to string
*/
print:
	lodsb
	or	%al, %al
	jz	1f
	mov	$0x0E, %ah
	mov	$0x00, %bh
	int	$0x10 /* execute instruction */
	jmp	print
1:
	ret	

/* some bootloader messages */
welcome_message: 	.string "CHB "
general_error: 		.string "ERROR."
newline: 			.string "\r\n"

/* this indicates the MBR start in some hard disks */
. = _start + MBR_START

.org MBR_END
.word BOOT_SIGNATUTE
