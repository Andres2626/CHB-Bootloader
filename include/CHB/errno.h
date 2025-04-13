
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

#define EUNKNOUN -1 /* unknoun error */
#define ENO      0  /* no error */
#define ESEC     1  /* invalid sector */
#define EREAD    2  /* read error */
#define EDEV     3  /* device error */
#define ENOMEM   4  /* no enough memory */

#endif /* !_ERRNO_H_ */