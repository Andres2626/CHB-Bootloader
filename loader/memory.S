
/*
* memory.S -- get memory info from BIOS
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
	
	push %eax
	enter_protmode
	pop %eax
	
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
	mov $0x88, %ah
	int $0x15
	
	push %eax
	enter_protmode
	pop %eax
	
	mov %ebp, %esp 
	pop %ebp 
	ret
