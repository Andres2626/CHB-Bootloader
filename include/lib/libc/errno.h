
/*
 * errno.h -- CHB error library
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _ERRNO_H_
#define _ERRNO_H_ 1

#include "const.h"

PUBLIC int errno;

#define ENO         0   /* no error */
#define ESINV       1   /* Invalid disk read/write sector. */
#define EREAD       2   /* Read error. */
#define EDISK       3   /* Disk error. */
#define EDEVINT     4   /* Disk driver is not initialized */
#define ENOMEM      5   /* Not enough memory. */
#define EMFILE      6   /* Maximum number of opened files exceeded. */
#define EKERN       7   /* Kernel not found */
#define EFATTR      8   /* Filesystem file attribute is invalid. */
#define EKERNLOC    9   /* Kernel localization in before 1MB is not supported. */
#define EMEMINIT    10  /* Memory driver is not initialized  */
#define EIFS        11  /* Invalid filesystem */
#define EREGFS      12  /* Number of registered filesystems exceeded */
#define ENOIMP      254 /* Not implemented */

#define EUNKNOUN    255 /* Unknoun error */

PROTOTYPE(void set_errno, (int error));

#endif /* !_ERRNO_H_ */