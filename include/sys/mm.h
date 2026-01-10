
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

/* upper memory definitions */
#define UPPER_MEMORY_BEGIN 0x7CFFE
#define UPPER_MEMORY_SIZE 0x5FFFF

#define FS_DRIVER_DATA      0x10300
#define FS_DRIVER_DATA_SIZE 0x2D00
#define KERN_SIZE 0x10000
#define KERN_BUFFER 0x13000
#define KERN_LOC 0x100000

#define MM_MAX_REGIONS 255
#define MM_RESERVED_SPACE 0x800
#define MM_BUFFER (UPPER_MEMORY_BEGIN - ((24 * MM_MAX_REGIONS) + MM_RESERVED_SPACE))

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

/* get all memory blocks via INT15,E820*/
PUBLIC int memory_init(struct memory_info *info);

#endif /* !_MM_H_ */