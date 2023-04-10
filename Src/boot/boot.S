
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
#include <boot/offsets.h>

.text

.code16

.global _start
_start:
	jmp codestart
	nop

.org 4 /*start of BPB*/

/*
* In FAT/NTFS filesystems, the bootloader reserves first 64-100 bits
* In this section the bootloader is compatible with FAT/NTFS filesystems
*/

.org 100 /*the end of BPB*/

drive:
	.byte 0
disk_address_packet:
	.word 0x0010			
stage2_size:
	.byte 4		
stage2_offset:		
	.word 0x8000
stage2_segment:
	.word 0x0
stage2_sector:
	.quad 1
/*END of DAP*/


codestart:
	/*this code jump to the correct location (in case of some bogus BIOS)*/
	ljmp $0, $realcode
	jmp kill_program

realcode:
	/*setup stack*/
	xor %ax,%ax
	mov %ax,%ds
	mov %ax,%es
	
	/*move stack from espected location*/
	mov $0x7c00,%sp
	
	/*clear display via INT10,0*/
	xor %ah, %ah
	mov $0, %bx
	mov $3, %al
	int $0x10
	
	MSG(loading)
	MSG(newline)
	
	/*get disk parameters via INT13,8*/
	push %es
	mov $0x8, %ah
	movb drive, %dl
	stc
	int $0x13
	jc floppy_error
	pop %es
	
	/*get sectors per head/track*/
	and $0x3f, %cl
	xor %ch, %ch
	movw %cx, 8(%si)
	
	/*get head number*/
	inc %dx
	movw %dx, (%si)
	
	/*get cylinder*/
	mov %ch, %bl
	mov %cl, %bh
	shr	$6, %bh
	inc %bx
	movw 4(%si), %bx
	
	/*read disk from 'norm_read' function*/
	mov stage2_sector, %ax /*LBA Sector*/
	mov stage2_size, %cl /*set sectors to read*/
	movw stage2_offset, %bx /*set buffer location*/
	movb drive, %dl /*set current disk number*/
	call norm_read

setup_stage2:
	movb drive, %dl /*set current disk number*/
	
	/*jump to stage2*/
	ljmp $0, $0x8000

/*errors*/
read_error:
	ERR(read)
	jmp boot_error

floppy_error:
	ERR(floppy)
	jmp boot_error

/*this function tell to the firmware 'continue booting.'*/
boot_error:
	int $0x18

/*stop the CPU*/
kill_program:
	jmp kill_program

loading: 	.string "CHB"
read: 		.string "READ"
floppy:		.string "FLOPPY"
error: 		.string " Error."
newline: 	.string "\r\n"

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


.org MBR_OFFSET_BEGIN

/*Magic number of MBR*/
mbr_magic:
	.long 0
	.word 0

.org STACK_SIZE
.word BOOT_SEC_MAGIC