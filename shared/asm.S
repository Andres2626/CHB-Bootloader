.title asm.S -- BIOS routines

#define ASM_FILE
#include "shared/switch_cpu.inc"
#include "shared/segments.inc"
#include "asm/asm_macros.h"

/*
 * uint8_t get_drive_number()
 * get the drive number from %dl register
 */
FUNCTION(get_drive_number)
	mov %dl, (%si)
	mov (%si), %eax
	ret

/*
 * void set_video_mode(int mode)
 * set VGA video mode with INT10,0
 */
FUNCTION(set_video_mode)
	push %ebp
	mov %esp, %ebp

	enter_realmode

	xor %ah, %ah # function 0
	mov 8(%bp), %al # get mode
	int $0x10
	
	enter_protmode

	mov %ebp, %esp
	pop %ebp
	ret
	
/*
 * int get_lower_memory(void)
 * get the size of low memory in KB via INT12
 */
FUNCTION(get_lower_memory)
	push %ebp
	mov %esp, %ebp
	
	pushw %si
	
	enter_realmode

	clc
	int $0x12
	jnc 2f
1:
	mov $-1, %eax
	jmp 3f
2:
	mov %ax, (%si)
	mov (%si), %eax
3:
	push %eax

	enter_protmode

	pop %eax
	
	popw %si

	mov %ebp, %esp
	pop %ebp
	ret
	
/*
 * int get_upper_memory(void)
 * get the size of extended memory in KB via INT15,88
 */
FUNCTION(get_upper_memory)
	push %ebp
	mov %esp, %ebp
	
	pushw %si
	
	enter_realmode
	
	mov $0x88, %ah /* function 88 */
	int $0x15
	
	jnc 3f
1:
	mov $-1, %ax /* error in call */
	jmp 3f
3:
	mov %ax, %bx

	enter_protmode
	
	mov %bx, %ax
	
	popw %si

	mov %ebp, %esp
	pop %ebp
	ret

/*
 * int get_memory_block(memory_block* block, uint32_t* continuation)
 * get the memory block viaINT15,e280
 */
FUNCTION(get_memory_block)
	push %ebp
	mov %esp, %ebp
	 
	enter_realmode
	
	push %ebx
	push %ecx
	push %edx
	push %esi
	push %edi
	push %ds
	push %es
	
	/* get structure argument */
	linear_to_segment 8(%bp), %es, %edi, %di
	
	/* get the continuation argument*/
	linear_to_segment 12(%bp), %ds, %esi, %si
	mov %ds:(%si), %ebx
	
	mov $0xe820, %eax /* function e280. */
	mov $0x534D4150, %edx /* signature */
	mov $24, %ecx /*size of structure*/
	
	int $0x15
	
	/* call fails */
	jc 2f

	cmp $0x534D4150, %eax
	jne 2f
1:
	mov %ecx, %eax
	mov %ebx, %ds:(%si)
	jmp 3f
2:
	mov $-1, %eax
3:
	pop %es
	pop %ds
	pop %edi
	pop %esi
	pop %edx
	pop %ecx
	pop %ebx
	
	push %eax

	enter_protmode

	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret

/*
 * int disk_reset(int drive_number)
 * reset disk controller with INT13, 0
 */
FUNCTION(disk_reset)
	push %ebp
	mov %esp, %ebp

	enter_realmode
	
	xor %ah, %ah /* function 0 */
	mov 8(%bp), %dl /* get drive */
	int $0x13
	
	mov $1, %eax
	sbb $0, %eax
	
	push %eax
	
	enter_protmode

	pop %eax

	mov %ebp, %esp
	pop %ebp
	ret

/*
 * int disk_read(uint8_t drive_number, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t sector_count, void* buffer)
 * reset disk controller with INT13, 0
 */
FUNCTION(disk_read)
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

/*
 * int disk_parameters(uint8_t drive_number, uint8_t* type, uint16_t* cylinder, uint16_t* sector, uint16_t* head)
 * reset disk controller with INT13, 0
 */
FUNCTION(disk_parameters)
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
	
/*
 * void outb(uint16_t port, uint8_t value)
 */
FUNCTION(outb)
	mov 4(%esp), %dx
	mov 8(%esp), %al
	out %al, %dx
	ret

/*
 * uint8_t inb(uint16_t port)
 */
 
FUNCTION(inb)
	mov 4(%esp), %dx
	xor %eax, %eax
	in %dx, %al
	ret
	