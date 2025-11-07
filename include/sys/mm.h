
/*
 * mm.h -- memory sizes definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _MM_H_
#define _MM_H_              1

#include "const.h"
#include "types.h"

/* 0x0 - 0x499 -- REALMODE RESERVED */

/* 0x500 - 0x7AFF (0x7600) -- FILESYSTEM DRIVER */
#define FS_DRIVER_DATA      0x500
#define FS_DRIVER_DATA_SIZE 0x7600

/* 0x7B00 - 0x7BFF -- UNUSED MEMORY */

/* 0x7C00 - 0x7FFF -- STAGE1 PROGRAM, STACK AND RESERVED MEMORY */

/* 0x8000 - 0x7FFFF -- LOADER MEMORY */

/* 0x8000 - 0xFFFFF -- UPPER MEMORY (RESERVED) */

#define KERN_SIZE 0x10000
#define KERN_BUFFER 0x30000
#define KERN_LOC 0x100000

#include <stdint.h>

struct memory_block {
	uint64_t begin; 
	uint64_t len; 
    uint32_t type;
    uint32_t flag;
} __attribute__((packed));

struct memory_info {
	int count;
	int lo;
	int hi;
	struct memory_block *blk;
};

PUBLIC int _ASM get_lower_memory();
PUBLIC int _ASM get_upper_memory();
PUBLIC int _ASM get_memory_block();

#define MEMORY_MAX_REGIONS 255

/* get all memory blocks via INT15,E820*/
PUBLIC int memory_init(struct memory_info *info);

#endif /* !_MM_H_ */