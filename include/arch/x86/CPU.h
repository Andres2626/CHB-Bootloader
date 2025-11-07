
/*
 * CPU.h -- CPU asm definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _CPU_H_
#define _CPU_H_          1

/* x86 CPU defines DO NO EDIT THIS! */
/* protected mode -- CR0 */
#define CR0_ENABLE       1
#define CR0_DISABLE      0xfffffffe

/* 16-bit and 32-bit code/data segments */
#define PROT_16_CODE_SEG 0x18
#define PROT_32_CODE_SEG 0x8
#define PROT_32_DATA_SEG 0x10
#define PROT_16_DATA_SEG 0x20
#define REAL_16_CODE_SEG 0x0

#endif /* !_TYPES_H_ */