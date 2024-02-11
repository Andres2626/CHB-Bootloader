
#define ASM_FILE

#include <CHB/cpu.h>
#include <CHB/macros.h>

/* switch CPU to real mode and protected mode */
.macro enter_realmode

.code32
	cli  # disable interrupts
	/* enter to 16-bit protected mode */
	ljmp $PROT_16_CODE_SEG, $1f
	
1:
.code16
	/* setup new data segments */
	mov $PROT_16_DATA_SEG, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

	/* disable CR0 protection bit */
	mov %cr0, %eax
	and $CR0_DISABLE, %al
	mov %eax, %cr0
	
	/* jump to real mode */
	ljmp $REAL_16_CODE_SEG, $2f

2:
	/* setup data registers */
	xor %ax, %ax
	mov %ax, %ds
	mov %ax, %ss
	
.endm

.macro enter_protmode

.code16
	cli # disable interrupts
	
	/* enable CR0 protection bit */
	mov %cr0, %eax
	or $CR0_ENABLE, %al
	mov %eax, %cr0
	
	/* enter to 32-bit protected mode */
	ljmp $PROT_32_CODE_SEG, $(1f)
	
.code32
1:
	/* setup protected mode data registers*/
	mov $PROT_32_DATA_SEG, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	
.endm
