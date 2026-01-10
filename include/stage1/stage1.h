
/*
 * stage1.h -- stage1 ASM definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STAGE1_H_
#define _STAGE1_H_          1

/* bootsector size (without signature) */
#define BOOT_PART_SIZE      510

/* magic boot number */
#define BOOT_MAGIC          0xaa55

/* BIOS parameter block start */
#define BOOT_BPB_START      0x4

/* BIOS parameter block end */
#define BOOT_BPB_END        0x3C

/* define the total bootblock size */
#define BOOT_TOTAL_SIZE     512

/* define the localization of stack */
#define BOOT_LOC            0x7c00

/* define the localization of disk buffer */
#define BOOT_DISK_BUFFER    0x2000

/* define the MBR start */
#define MBR_START           0x1b8

/* define MBR partiion start */
#define MBR_PARTITION_START 0x1be

/* stage1 stack */
#define BOOT_STACK          0x7c00

/* CHB magic number */
#define CHB_MAGIC 0x93A76FD8

#endif /* !_STAGE1_H_ */