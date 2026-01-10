.title stage1.S -- BOOTSECTOR routines

.file "stage1_bootsec.S"  

.code16

#include "stage1.h"
#include "asm/asm_macros.h"

.globl _start
_start:
	/* some filesystems use an special code for work, this code ensure the compatibility */
	jmp	afther_bpb
	nop

.org BOOT_BPB_START	

	/* This is an space commonly used in some filesystems (FAT, NTFS or HPFS) */

.org BOOT_BPB_END

stage1_size:
	.word 24
drive_number:
	.byte 0

afther_bpb:
	cli # disable interrupts
	cld
	
	/* some bogus bios does'nt jump to the bootsector correcly, ljmp to 7c00:0 instead of 0:7c00*/
	ljmp $0, $real_start
real_start:
	/* save the drive number */
	movb %dl, drive_number

	/* setup stack */
	xor	%ax, %ax
	mov	%ax, %ss
	mov	$0x7c00, %sp # set the stack localization in memory
	call enable_a20 # enable a20 gate
	lgdt %ss:gdt # load GDT to jump to unreal mode
	
	/* set cr0 protection bit */
	mov	%cr0, %eax
	or $0x1, %al 
	mov	%eax, %cr0
.code32
	.byte 0x66 # no short opcode for this
	ljmp $0x8,$(unreal32)
	
.code16
unreal32:
	/* setup protected stack */
	mov	$0x10, %bx
	mov	%bx, %ds
	mov	%bx, %es
	mov	%bx, %ss
	
	/* set cr0 protection bit */
	and	$0xfe, %al
	mov	%eax, %cr0
	
	/* entry in unreal-mode */
	ljmp $0x0,$(unreal)

unreal:
	/* setup data registers for unreal mode */
	xor	%ax, %ax
	mov	%ax, %es
	mov	%ax, %ds
	mov	%ax, %ss

	/* disable interrupts */
	sti

	MSG(welcome_message)

	/* read the second part of bootloader */
	xor	%dx, %dx # set the number of head in 0
	mov	$0x2, %bx # read the second sector
	mov $0x1, %cx # read one sector
	mov	$0x7e00, %edi # set our buffer
	call load_floppy

	MSG(load_messsage)

	jmp setup_stage1

load_floppy_value:
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
	decb load_floppy_value # substract 1 to read retry number
	jz read_error # fail call
	xor %ah, %ah # function 0
	int $0x13 # reset floppy controller via INT13,0
	jmp load_floppy_loop
2:
	movb $3, load_floppy_value # reload retry number
	MSG(dot)
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

disable_floppy_motor:
	xor	%al, %al
	mov	$0x3f2, %dx  # disable floppy motor
	out	%al, %dx
	ret

bios_print:
	pusha
print_loop:
	lodsb
	or %al, %al
	jz 1f
	mov	$0x0e, %ah
	mov	$0x0, %bx
	int	$0x10
	jmp	print_loop
1:
	popa
	ret

read_error:
	MSG(read_error_message)
contunue_booting:
	int $0x18
kill_program:
	jmp kill_program

/* ensure a20 is enabled */
enable_a20:
	call enable_a20_loop
	jnz enable_a20_done
	mov $0xd1, %al
	out %al, $0x64
	call enable_a20_loop
	jnz enable_a20_done
	mov $0xdf, %al
	out %al, $0x60
enable_a20_loop:
	mov $0x20000, %ecx
enable_a20_loop2:
	jmp enable_a20_
enable_a20_:
	in $0x64, %al
	test $0x2, %al
	loopne enable_a20_loop2
enable_a20_done:
	ret

welcome_message:
	.string "CHB"
load_messsage:
	.string	" Loading"
read_error_message:
	.string	"\r\nDisk read fail.\r\n"
newline:
	.string "\r\n"
dot:
	.string	"."
gdt:
	.word 0xffff
	.word gdt
	.long 0
	.long 0x0000ffff
	.long 0x008f9a00
	.long 0x0000ffff
	.long 0x008f9200

/* mark the MBR start */
.org MBR_START

/* magic number used in MBR */
mbr_magic_number:
	.long 0
	.word 0

.org MBR_PARTITION_START

.org BOOT_PART_SIZE
.word BOOT_MAGIC # magic number for booting systems

setup_stage1:
	/* read the c-like stage1 bootloaader*/
	mov	$0x8200, %edi # set buffer
	mov	$0x3, %bx # read third sector
	mov stage1_size, %cx
	xor	%dx, %dx # set head to 0
	call load_floppy # read sectors
	call disable_floppy_motor
	
	/* disable interrups */
	cli

	/* back to protected mode */
	mov	$0xcf, %al
	mov	%al, (gdt + 14)
	mov	%al, (gdt + 22) # 32-bit segment
	lgdt gdt # load global descriptor table

	/* enable cr0 protection bit */
	mov	%cr0, %eax
	or $1, %al
	mov	%eax,%cr0

.code32
	.byte 0x66 # no short opcode for this
	ljmp $0x8,$(code32) # enter to the 32-bit mode

code32:
	# set the data segments
	mov	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	mov	$0x10000,%ebp
	mov	%ebp, %esp
	push %eax
	
	/* set number of drive in %dl */
	xor %edx, %edx
	movb drive_number, %dl
	push %edx

	mov $0x8200, %ebx
	call *%ebx
stop:
	jmp	stop

.org 1024

