
#define ASM_FILE
#define LOADER_FILE

#include <CHB/stage1/stage1.h>
#include <CHB/macros.h>

/*
 * TODO:
 * - Add code for call C code (cmain function)
 * - Add support for Hard disk and CDROMS
 * - Implement full suport for extended disk read mode (INT13,42)
 */


.file "loader.S"

.code16

SECTION(text)

.global _start
_start:
	jmp real_start
	
.org 0x8

drive_number:
	.byte 0
kernel_loc:
	.long 0x100000
kernel_sector_loc:
	.word 5 /* TODO: Get this data dinamically */
kernel_size:
	.word 5

.org 0x24
	
real_start:
	/* print welcome message */
	MSG(welcome_message)

	/* save the drive number */
	movb %dl, drive_number

	/* disable interrupts */
	cli
	lgdt gdt
		
	/* set cr0 protection bit */
	mov	%cr0, %eax
	or $0x1, %al 
	mov	%eax, %cr0
	
	/* long jump to 16-bit prot mode */
	ljmp $0x8, $pmode
pmode:
	/* setup protected stack */
	mov	$0x10, %bx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %ss
	mov	%bx, %ds
	
	and $0xfe, %al
	mov %eax, %cr0
	
	ljmp $0, $umode
	
/* 
 * in some kernels have located in 0x100000 (1MB) for load this, enable unreal mode,
 * read the kernel at 1MB and change to protected mode.
 */
umode:
	xor %ax, %ax
	mov %ax, %es
	mov %ax, %ds
	mov %ax, %ss
	
	/* restore interrupts */
	sti

	/* read kernel at fifth sector */
	xor %dx, %dx
	mov kernel_sector_loc, %bx
	mov kernel_size, %cx
	mov kernel_loc, %edi
	call load_floppy
	
	/* Congratulations!, your kernel has loaded in protected mode!!! */
	call sucsess_load
	
	cli
	
	mov $0xcf, %al
	mov %al, (gdt + 14)
	mov %al, (gdt + 22)
	lgdt gdt
	
	/* set cr0 protection bit */
	mov	%cr0, %eax
	or $0x1, %al 
	mov	%eax, %cr0

	ljmp $0x8, $exec_kernel

.code32

exec_kernel:
	mov	$0x10, %bx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %ss
	mov	%bx, %ds
	
	/* execute kernel */
	mov $0x100000, %ebx
	call *%ebx
	
	jmp stop

.code16

load_retry:
	.byte 3
load_floppy:
	push %bx # number of sector
	push %cx # sectors to read
load_floppy_loop:
	mov	$0x13, %al # read only 18 sectors because floppy geometry
				   # TODO: get floppy geometry.
	sub	%bl, %al
	xor	%ah, %ah
	cmp	%cx, %ax
	jl 1f
	mov	%cx, %ax
1:
	mov %bx, %cx
	mov $BOOT_DISK_BUFFER, %bx # buffer address
	mov $0x2, %ah # function 2
	push %ax
	int $0x13 # read sectors via INT13,2
	pop %ax
	jnc 2f # sucsess call
	decb load_retry # substract 1 to read retry number
	jz read_error # fail call
	xor %ah, %ah # function 0
	int $0x13 # reset floppy controller via INT13,0
	jmp load_floppy_loop
2:
	movb $3, load_retry # reload retry number
	mov $BOOT_DISK_BUFFER, %esi # set the buffer
	xor %ecx, %ecx
	mov %al, %cl
	shl $0x7, %cx
	rep addr32 movsd # mov %ds:%esi to %es:%edi
	pop %cx
	pop %bx
	xor $0x1, %dh # read next head
	jnz 3f
	inc %bh # read next cylinder
3:
	mov $0x1, %bl # set sector in 1
	xor %ah, %ah
	sub %ax, %cx
	jg load_floppy # read the left sectors
	ret

sucsess_load:
	MSG(sucsess_load_msg);
	MSG(newline_msg)
	ret
read_error:
	MSG(read_error_msg)
boot_error:
	int $0x18
stop:
	jmp stop

read_error_msg: .string "Error reading disk. "
sucsess_load_msg: .string " YES!"
welcome_message: .string "Loading Kernel..."
newline_msg: .string "\r\n"

_print:
	pusha
1:
	lodsb
	or %al, %al
	jz 2f
	mov	$0x0e, %ah
	mov	$0x0, %bx
	int	$0x10
	jmp	1b
2:
	popa
	ret

.p2align 2
gdt:
	.word 0xffff
	.word gdt
	.long 0
	.long 0x0000ffff
	.long 0x008f9a00
	.long 0x0000ffff
	.long 0x008f9200
	