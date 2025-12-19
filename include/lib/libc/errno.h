
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

#define SIGN(x) -x

#define ENO         0   /* no error */
#define ENOMEM      1   /* No enough memory */
#define EMFILE      2   /* Maximum number of opened files exceeded. */
#define EKERN       3   /* Kernel not found */
#define EIFS        4   /* Invalid filesystem */
#define EREGFS      5   /* Number of registered filesystems exceeded */
#define EIO         6   /* Input/Output error */
#define ENOEXEC     7   /* Executable format error */
#define EINVAL      8   /* Invalid value */
#define EFAULT      9   /* Invalid pointer */
#define ESYSINIT    10  /* Subsystem not initialized */
#define EFILE       11  /* File not found */
#define EKERNLOC    12  /* Kernel localization before 1MB is not supported */
#define EFS         13  /* Filesystem error */
#define ENOIMP      254 /* Not implemented */

#define EUNKNOWN    255 /* Unknoun error */

#endif /* !_ERRNO_H_ */