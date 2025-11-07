
/*
* blib.S -- bootloader ASM helper functions
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE

#include "arch/x86/asm/CPU.S"
#include "arch/x86/macros.h"

.text

.code32

.macro linear_to_segment bp, es, esi, si 
	mov \bp, \esi 
	shr $0x4, \esi 
	mov \si, \es 
	mov \bp, \esi 
	and $0xf, \esi
.endm

/* 
* Sets the current video mode via INT10,0.
*
* Note that this function is used to ensure that the video mode 
* delivered by the BIOS is correct (80x25); This mode is 0x3.
*
* TODO: Implement different video modes 
*
* function: int video_driver_set_mode(uint8_t mode)
*/
FUNCTION(video_driver_set_mode) 
	push %ebp 
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah #function 0 
	movb 8(%bp), %al #get mode int $0x10
	int $0x10

	enter_protmode

	mov %ebp, %esp 
	pop %ebp 
	ret

.code32

/*
* Reset specified disk controller via INT13,0
* 
* function: int disk_reset_controller(uint8_t)
*/
FUNCTION(disk_reset_controller) 
	push %ebp 
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah /* function 0 */
    mov 8(%bp), %dl /* get drive number */
    stc 
	int $0x13
    jc 1f
	jmp 2f
1:
    mov $0, %ax
2:	

    mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax

	mov %ebp, %esp 
	pop %ebp 
	ret

/*
* Get drive properties via INT18,8
* 
* function: int disk_get_parameters(uint8_t, uint16_t*, uint16_t*, uint16_t*, uint16_t*)
*/
FUNCTION(disk_get_parameters) 
	push %ebp 
	mov %esp, %ebp

	enter_realmode

	push %es 
	push %bx 
	push %esi 
	push %di

	mov 8(%bp), %dl /* get drive number */
	mov $0x8, %ah /* function 8.*/
	mov $0, %di 
	mov %di, %es /* buffer at 0000:0000*/
	stc 
	int $0x13

	/* set return value */
	mov $1, %eax 
	sbb $0, %eax /* in case of error return 0 */

	/* TWO ARGUMENT: disk type */
	linear_to_segment 12(%bp), %es, %esi, %si 
	mov %bl, %es:(%si)

	mov %ch, %bl 
	mov %cl, %bh 
	shr $6, %bh 
	inc %bx

	/* THREE ARGUMENT: cylinders */
	linear_to_segment 16(%bp), %es, %esi, %si 
	mov %bx, %es:(%si)

	xor %ch, %ch 
	and $0x3f, %cl

	/* FOUR ARGUMENT: sectors */
	linear_to_segment 20(%bp), %es, %esi, %si 
	mov %cx, %es:(%si)
	mov %dh, %cl 
	inc %cx

	/* FIVE ARGUMENT: heads */
	linear_to_segment 24(%bp), %es, %esi, %si 
	mov %cx, %es:(%si)

	pop %di 
	pop %esi 
	pop %bx 
	pop %es

	mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax

	mov %ebp, %esp 
	pop %ebp 
	ret

/*
* Read floppy via INT13,2
* 
* function: int disk_read(uint8_t, uint16_t, uint16_t, uint16_t, uint8_t, void*)
*/
FUNCTION(disk_read) 
	push %ebp 
	mov %esp, %ebp

	enter_realmode

	push %ebx 
	push %es

	mov 8(%bp), %dl /* %dl - number of drive */
	mov 12(%bp), %ch /* %ch - cylinder lower 8 bits */
	mov 13(%bp), %cl /* %cl - cylinder upper */
	shl $6, %cl
	mov 16(%bp), %al 
	and $0x3f, %ah 
	or %al, %cl
	mov 20(%bp), %dh /* %dh - head number */
	mov 24(%bp), %al /* %al - sectors to read */

	linear_to_segment 28(%bp), %es, %ebx, %bx /* buffer */

	mov $2, %ah 
	stc 
	int $0x13

	/* set return value */
	mov $1, %eax 
	sbb $0, %eax /* in case of error return 0 */

	pop %es 
	pop %ebx

	mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax

	mov %ebp, %esp 
	pop %ebp 
	ret

/* 
* Return size of contigous memory starting in 0x0 via INT12
*
* Note: This function only detects usable memory before 1MB of memory, 
* usually it always returns a value less than 640KB, All memory before
* this is reserved.
*
* Function: int get_lower_memory()
*/
FUNCTION(get_lower_memory)
	push %ebp 
	mov %esp, %ebp

	enter_realmode
	
	/* clear carry flag */
	clc
	
	int $0x12
	
	/* set return value */
	mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax
	
	mov %ebp, %esp 
	pop %ebp 
	ret

/*
* Return size of usable memory starting in 1MB via INT15,88
*
* Note: This function in some BIOS only detects up to the ISA 
* memory hole (15 MB).
*
* Function: int get_upper_memory()
*/
FUNCTION(get_upper_memory)
	push %ebp 
	mov %esp, %ebp

	enter_realmode
	
	/* clear carry flag */
	clc
	
	/* function 0x88 */
	mov $0x88, %ah
	int $0x15
	jc 1f /* error */
	jmp 2f
1:
	mov $-1, %ax /* return invalid memory value */
2:
	mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax
	
	mov %ebp, %esp 
	pop %ebp 
	ret
	
/*
* Return memory block via INT15,E820
*
* Function: int get_memory_block(struct memory_block *blk, u32t *continuation)
*/
FUNCTION(get_memory_block)
	push %ebp 
	mov %esp, %ebp

	enter_realmode
	
	/* save all registers */
	push %ebx
	push %ecx
	push %edx
	push %esi
	push %edi
	push %ds
	push %es
	
	/* first argument*/
	linear_to_segment 8(%bp), %es, %edi, %di
	
	/* second argument */
	linear_to_segment 12(%bp), %ds, %esi, %si
    mov %ds:(%si), %ebx
	
	/* setup registers*/
	mov $0xE820, %eax
	mov $0x534D4150, %edx
	mov $20, %ecx
	int $0x15
	
	/* error */
	cmp $0x534D4150, %eax
	jne 1f
success:
	mov %ecx, %eax
	mov %ebx, %ds:(%si)
	jmp 2f
1:
	mov $-1, %ax /* return invalid value */
2:
    /* restore all registers */
	pop %es
	pop %ds
	pop %edi
	pop %esi
	pop %edx
	pop %ecx
	pop %ebx
	
	/* set return value */
	mov %ax, %bx
	
	enter_protmode
	
	mov %bx, %ax
	
	mov %ebp, %esp 
	pop %ebp 
	ret
	
/* void port_outb(uint16_t port, uint8_t value) */
FUNCTION(port_outb)
	movw 4(%esp), %dx 
	movb 8(%esp), %al 
	out %al, %dx 
	ret

/* uint8_t port_inb(uint16_t port) */
FUNCTION(port_inb)
	movw 4(%esp), %dx 
	xorl %eax, %eax 
	in %dx, %al 
	ret

/* void stop_system() */
FUNCTION(stop_system)
    enter_realmode

real_stop:
    jmp real_stop

/* void reboot() */
FUNCTION(reboot)
    enter_realmode
	xor %ax, %ax
	int $0x16
	ljmp $0xFFFF, $0x0000

FUNCTION(int19)
    enter_realmode
	xor %ax, %ax /* function 0 */
	int $0x16
	int $0x19
	
	/* if 0x19 fails jmp to infinite loop */
	jmp real_stop

FUNCTION(enable_ints)
    sti
	ret
	
FUNCTION(disable_rets)
    cli
	ret
    


