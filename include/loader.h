
/*
 * loader.h -- loader definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _LOADER_H_
#define _LOADER_H_ 1

#include <types.h>

/* magic number for kernel */
#define MSYS_KERN_MAGIC 0x3F8A857B

/* size of header for 0.06 - 0.08 versions */
#define CHB_HDR_SIZE_V06 0x5C 

/* 
 * This is CHB header used by C part for detect some reported info
 * Offset  Size     Description
 * 0x0     0x4      CHB magic number, must be '0x93A76FD8' for sucessful load.
 * 0x4     0x4      Header size. (must be 0x5C for v0.06)
 * 0x8     0x1      Reported disk number (BIOS)
 * 0x9     0x3      Version number (byte 0: Major, byte 1: Minor, byte 2: Revision)
 * 0xC     0x10     Version string.
 * 0x1C    0x20     First option that CHB can boot from FS.
 * 0x3C    0x20     Second boot option TODO: Not implemented.
 */
struct loader_hdr {
    u32t magic;
    u32t size;
    u8t drive_number;
    u8t version_num[3];
    char version[16];
    char first_boot[32];
    char second_boot[32];
    
    /* reserved: not implemented yet! */
} __attribute__(( packed ));

#ifndef CHB_UTIL

#include <sys/disk.h>
#include <sys/mm.h>

struct msys_kern_hdr {
    u32t magic;
	struct device disk; /* disk properties */
	struct memory_info mem; /* memory info */
	u8t vid_mode; /* video BIOS mode */
};

#endif

#endif /* !_LOADER_H_ */