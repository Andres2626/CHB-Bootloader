
/*
* port.S -- asm x86 port functions
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE
#include <CHB/macros.h>

/* void port_outb(uint16_t port, uint8_t value) */
FUNCTION(port_outb)
	mov 4(%esp), %dx 
	mov 8(%esp), %al 
	out %al, %dx 
	ret

/* uint8_t port_inb(uint16_t port) */
FUNCTION(port_inb) 
	mov 4(%esp), %dx 
	xor %eax, %eax 
	in %dx, %al 
	ret
