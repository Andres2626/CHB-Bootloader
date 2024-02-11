
#define ASM_FILE
#include "CPU.S"
#include <CHB/macros.h>

.text

.code32

/*
 * uint8_t video_set_mode(uint8_t mode);
 * set VGA video mode with INT10,0
 */
FUNCTION(video_set_mode)
	push %ebp
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah # function 0
	movb 8(%bp), %al # get mode
	int $0x10
	
	mov 8(%bp), %eax
	push %eax
	
	enter_protmode
	
	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret
	