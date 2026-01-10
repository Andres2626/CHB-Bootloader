.title gdt.S -- Global Descriptor Table functions

.code32

#include "asm/asm_macros.h"

/* 
* The bootsector provides a GDT by default, this code load new GDT for switch 
* real/protected mode easily.
*/
FUNCTION(load_new_gdt)
	lgdt gdt_descriptor
	ret

.p2align 2
gdt:
	.quad 0 # null descriptor
	
	/* 32-bit code segment */
	.word 0xFFFF, 0
	.byte 0
	.byte 0b10011010, 0b11001111, 0
	
	/* 32-bit data segment */
	.word 0xFFFF, 0
	.byte 0
	.byte 0b10010010, 0b11001111, 0
	
	/* 16-bit code segment */
	.word 0xFFFF, 0
	.byte 0
	.byte 0b10011010, 0b00001111, 0
	
	/* 16-bit data segment */
	.word 0xFFFF, 0
	.byte 0
	.byte 0b10010010, 0b00001111, 0
gdt_end:
VARIABLE(gdt_descriptor)
	.word gdt_end - gdt
	.long gdt
	