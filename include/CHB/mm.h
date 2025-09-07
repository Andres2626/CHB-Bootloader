
/*
 * mm.h -- memory sizes definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _MM_H_
#define _MM_H_              1

/* 0x0 - 0x499 -- REALMODE RESERVED */

/* 0x500 - 0x7AFF (0x7600) -- FILESYSTEM DRIVER */
#define FS_DRIVER_DATA      0x500
#define FS_DRIVER_DATA_SIZE 0x7600

/* 0x7B00 - 0x7BFF -- UNUSED MEMORY */

/* 0x7C00 - 0x7FFF -- STAGE1 PROGRAM, STACK AND RESERVED MEMORY */

/* 0x8000 - 0x7FFFF -- LOADER MEMORY */

/* 0x8000 - 0xFFFFF -- UPPER MEMORY (RESERVED) */

#define KERNEL_LOCALIZATION 0x100000

#endif /* !_MM_H_ */