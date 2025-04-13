
/*
* int10_handler.S -- video access
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
	mov 8(%bp), %eax 
	push %eax

	enter_protmode

	pop %eax 
	mov %ebp, %esp 
	pop %ebp 
	ret
