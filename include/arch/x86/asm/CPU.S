
/*
* CPU.S -- CPU switching macros
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#define ASM_FILE

#include <arch/x86/CPU.h>

/* switch CPU to real mode and protected mode */
.macro enter_realmode
.code32
	call switch_to_real_mode
.code16
.endm

.macro enter_protmode
.code16
	call switch_to_prot_mode
.code32
.endm

.macro GDT_ENTRY base, limit, access, flags
    .word (\limit) & 0xFFFF
    .word (\base) & 0xFFFF
    .byte ((\base) >> 16) & 0xFF
    .byte \access
    .byte (\flags << 4) | ((\limit >> 16) & 0xF)
    .byte ((\base) >> 24) & 0xFF
.endm
