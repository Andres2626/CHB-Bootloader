
#define ASM_FILE
#include <CHB/macros.h>
#include "CPU.S"

/* TODO: fully support for INT13,4X */

/* reset drive system via INT13,0 */
FUNCTION(drive_reset_controller)
	push %ebp
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah /* function 0 */
	mov %dl, 8(%bp) /* get drive number */
	int $0x13
	
	mov $1, %eax
	sbb $0, %eax
	
	enter_protmode
	
	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret

/* get drive parameters via INT13,8 */
FUNCTION(drive_get_parameters)
	push %ebp
	mov %esp, %ebp

	enter_realmode
	
	push %es
	push %bx
	push %esi
	push %di

	mov 8(%bp), %dl /* %dl - drive number */
	mov $0x8, %ah /* function 8.*/
	mov $0, %di
	mov %di, %es /* buffer at 0000:0000*/
	int $0x13
	
	mov $1, %eax
	sbb $0, %eax
	
	linear_to_segment 12(%bp), %es, %esi, %si
	mov %bl, %es:(%si)
	
	mov %ch, %bl
	mov %cl, %bh
	shr $6, %bh
	inc %bx
	
	linear_to_segment 16(%bp), %es, %esi, %si
	mov %bx, %es:(%si)

	xor %ch, %ch
	and $0x3f, %cl

	linear_to_segment 20(%bp), %es, %esi, %si
	mov %cx, %es:(%si)

	mov %dh, %cl
	inc %cx
	
	linear_to_segment 24(%bp), %es, %esi, %si
	mov %cx, %es:(%si)

	pop %di
	pop %esi
	pop %bx
	pop %es

	push %eax

	enter_protmode

	pop %eax
	
	/*pop call frame*/
	mov %ebp, %esp
	pop %ebp
	ret

/* read drive via INT13,2 */
FUNCTION(drive_read)
	push %ebp
	mov %esp, %ebp

	enter_realmode
	
	push %ebx
	push %es
	
	mov 8(%bp), %dl /* %dl - number of drive */
	mov 12(%bp), %dh /* %ch - cylinder lower 8 bits */
	mov 13(%bp), %cl /* %cl - cylinder upper */
	shl $6, %cl
	
	mov 16(%bp), %al
	and $0x3f, %ah
	or %al, %cl
	
	mov 20(%bp), %dh /* %dh - head number */
	mov 24(%bp), %al /* %al - sectors to read */
	
	linear_to_segment 28(%bp), %es, %ebx, %bx /* buffer */
	
	mov $2, %ah
	int $0x13
	
	mov $1, %eax
	sbb $0, %eax
	
	pop %es
	push %ebx
	
	push %eax
	
	enter_protmode

	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret
	