
/*
 * errno.h -- CHB error library
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _ERRNO_H_
#define _ERRNO_H_ 1

extern int errno; /* defined in libc/string/strerror.c */

#define ERR_UNKNOUN -1 /* Unknoun error. */
#define ERR_NO 0 /* No error. */
#define ERR_SECTOR_INV 1 /* Invalid disk read/write sector. */
#define ERR_DISK_READ 2 /* Disk reading error. */
#define ERR_DISK_ERR 3 /* Disk error. */
#define ERR_DEVICE_INIT 4 /* Device is not initialized */
#define ERR_NO_MEM 5 /* Not enough memory. */
#define ERR_TOO_FILES 6 /* Maximum number of opened files exceeded. */
#define ERR_KERN_NO 7 /* Kernel not found in filesystem */
#define ERR_FSYS_ATTR 8 /* Filesystem file attribute is invalid. */
#define ERR_KERN_LOC 9 /* Kernel localization in before 1MB is not supported. */

void set_errno(int error);

#endif /* !_ERRNO_H_ */