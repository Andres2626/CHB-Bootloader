
/*
*	 CHB - Computers Help Bootloader
*    Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023  Andres26, Computers Help
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boot/boot.h>

.text

.code16

.global _start
_start:
	jmp codestart
	nop 
	
/*
* At the moment CHB only reads determined number of sectors for boot your kernel
* TODO: 
*		- Implement filesystems, executable formats (ELF) and relocations for execute your
* 		  kernels.
*		- Implement the a kind of multiboot so you can load multiple kernels 
*		- Implement the extended mode (LBA mode) for more compatibility
*		- Implement a way to configure the bootloader to be able to run kernels automatically
*		- Implement compatibility with HDD and CDROM drives 
*/	

/*This kernel parameters you can edit it at will*/
kernel_size:
	.byte 4
/*
* when the a20 are enabled you can use the high memory area for access to your kernel
* note that you have to enable unreal mode though don't worry CHB bootloader does this 
* for you
*/
kernel_offset:		
	.word 0x10	/*offset: 0x10*/
kernel_segment:
	.word 0xFFFF /*segment 0xFFFF*/

/*The CHB Bootloader for default charges your kernel in the physical address 0x100000, 0xFFFF:0x10, first
jump to unreal mode, load the kernel, jump to protected mode and pass the control to your kernel */
	
/*your kernel has to be relocated after stage2*/
kernel_sector:
	.quad 5

/*stage2 begin here*/	
codestart:
	/*setup stack*/
	xor %ax,%ax
	mov %ax,%ds
	mov %ax,%ss
	
	/*move stack from espected location*/
	mov $0x8000, %sp

	/*save %si, in %di*/
	mov %si, %di

	/*stage2 loading message*/
	MSG(loading)
	MSG(newline)
	
	/*prepare to jump to unreal mode*/
	call enableA20
	
	/*disable interrupts*/
	cli
	cld
	
	/*save the real mode from stack!*/
	push %ds
	
	/*load the GDT*/
	lgdt gdt
	
	/*enable CR0 again*/
	mov	%cr0, %eax        
	or	$0x1, %al          
	mov	%eax, %cr0
	
	/*jump to the protected mode*/
	ljmp $0x8, $pmode

/*This tell to the compile use the 32 bit mode*/
.code32

pmode:
	/*set registers in correct value!*/
	mov $0x10, %bx
	mov %bx, %ds
	mov	%bx, %es
	mov	%bx, %ss

	/*enable CR0 again*/
	and $0xFE, %al
	mov	%eax, %cr0 /*set the CR0*/
	ljmp $0x0, $unreal
	
/*This tell to the compile use the 16 bit mode*/
.code16

/*Well, you're on unreal mode!*/
unreal:
	/*delete the real mode from stack!*/
	pop %ds
	
	/*set correct registers!*/
	xor	%ax, %ax            
	mov	%ax, %es            
	mov	%ax, %ds          
	mov	%ax, %ss
	
	sti /*restore interrupts*/
	
	MSG(kern_load)
	MSG(newline)
	
	/*read disk from 'norm_read' function*/
	mov kernel_sector, %ax /*LBA Sector*/
	mov kernel_size, %cl /*set sectors to read*/
	
	/*set the kernel segment*/
	mov kernel_segment, %bx 
	mov %bx, %es
	
	/*set the kernel offset*/
	mov kernel_offset, %bx
	
	/*read disk*/
	call norm_read
	
	/*stop the floppy motor*/
	xor	%al, %al
	mov	$0x3f2, %dx        
	out	%al, %dx

setup_kernel:
	/*print the boot message*/
	MSG(kern_boot)
	MSG(newline)

	/*disable interrupts*/
	cli

	/*edit the GDT to jump to protected*/
	mov	$0xcf, %al
	mov	%al, (gdt + 14)       
	mov	%al, (gdt + 22)

	/*load the GDT*/
	lgdt gdt

	/*enable CR0 again*/
	mov	%cr0, %eax        
	or	$0x1, %al          
	mov	%eax, %cr0
	
	/*jump to the protected mode again*/
	ljmp $0x8, $kernel_32

/*This tell to the compile use the 32 bit mode*/
.code32

kernel_32:
	/*set registers in correct value!*/
	mov $0x10, %bx
	mov %bx, %ds
	mov	%bx, %es
	mov	%bx, %ss

	mov $0x100000, %ebx
	call *%ebx
	
	jmp kill_program

/*This tell to the compile use the 16 bit mode*/
.code16

/*
* This code enable the A20 gate needed for jump to protected/unreal mode
* At the moment the enableA20 functions only works via BIOS INT15,2401
*/
enableA20:
	mov $0x2401, %ax
	int $0x15 /*probe a20 state with INT15, 2401*/
	jb a20_error /*if call fails print error and die*/
	cmp $0, %ah
	jnz a20_error
	ret
	
a20_error:
	MSG(a20)
	MSG(newline)
	
	/*kill program*/
	jmp boot_error

read_error:
	MSG(read)
	MSG(newline)
	
	/*kill program*/
	jmp boot_error

/*This function tell to the BIOS continue booting.*/
boot_error:
	int $0x18
	
/*
* infinite loop. i no use the cli and hlt instruction because in some hardware the CPU
* remains in dead point 
*/
kill_program:
	jmp kill_program

/*your messages*/
a20:		.string "Error enabling A20 gate."
loading:    .string "Loading CHB... Please Wait..."
kern_load:	.string "Loading Kernel at 0x100000"
kern_boot:	.string "Booting Kernel..."
read:		.string "Error reading disk."
newline: 	.string "\r\n"	

/*convert LBA sector sector to CHS*/
lba_to_chs:
	push   %ax
	push   %dx
	xor    %dx,%dx
	divw   8(%si)
	inc    %dx
	mov    %dx,%cx
	xor    %dx,%dx
	divw   (%si)
	mov    %dl,%dh
	mov    %al,%ch
	shl    $0x6,%ah
	or     %ah,%cl
	pop    %ax
	mov    %al,%dl
	pop    %ax
	ret

/*read disk with CHS mode via INT13,2*/
norm_read:
	push %ax
	push %bx
	push %cx
	push %dx
	push %di
	push %cx
	call lba_to_chs
	pop %ax
	mov $0x2,%ah
	mov $0x3,%di
	pusha
	stc
	int $0x13
	jc read_error
	popa
	pop %di
	pop %dx
	pop %cx
	pop %bx
	pop %ax
	ret

/*print string via INT10,0E*/
bios_print:
	lodsb
	or	%al, %al
	jz	1f
	mov	$0x0E, %ah
	mov	$0x00, %bh
	int	$0x10
	jmp	bios_print
1:
	ret	

/*
* This is a GDT (global descriptor table), needed for mangle memory 
* and jump to protected mode
*/
	
.p2align	2	

gdt:
	.word 0xffff       
	.word gdt         
	.long 0
	/*descriptor 1 16-bit code*/
	.long 0x0000ffff  
	.long 0x008f9a00  
	/*descriptor 2 16-bit data*/
	.long 0x0000ffff  
	.long 0x008f9200  

/*four sectors*/
.org 512 * 4

