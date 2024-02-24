
#define ASM_FILE
#include "CPU.S"
#include <CHB/macros.h>

.text

.code32

keystroke:
scan_code:
	.byte 0 
key:
	.byte 0

/*
 * uint16_t keyboard_get_key_status()
 * get keyboard status via INT16,0
 */
FUNCTION(keyboard_get_key_status)
	push %ebp
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah
	int $0x16
	
	mov %ax, (keystroke)
	mov (keystroke), %eax
	
	push %eax
	enter_protmode
	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret
	