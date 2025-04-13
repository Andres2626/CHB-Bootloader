
/*
* int13_handler.S -- disk routunes and access
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE
#include <CHB/macros.h>
#include "CPU.S"

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
* Reset specified disk controller via INT13,0
* 
* function: int disk_reset_controller(uint8_t)
*/
FUNCTION(disk_reset_controller) 
	push %ebp 
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah /* function 0 */
    mov %dl, 8(%bp) /* get drive number */
    stc 
	int $0x13

    /* set return value */
    mov $1, %eax 
	sbb $0, %eax /* in case of error return 0 */

	enter_protmode

	pop %eax

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

	mov 8(%bp), %dl /* ONE ARGUMENT: disk number */
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

	push %eax

	enter_protmode

	pop %eax

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
	push %ebx

	push %eax

	enter_protmode

	pop %eax

	mov %ebp, %esp 
	pop %ebp 
	ret
