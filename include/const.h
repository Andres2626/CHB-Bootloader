
/*
 * const.h -- const definitions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _CONST_H_
#define _CONST_H_ 1

#define PUBLIC extern
#define PRIVATE static

#define PROTOTYPE(func, param) func param

/* used for ASM calls */
#define _ASM __attribute__((cdecl))

#define _VOIDSTAR     void*
#define _VOID         void
#define _CONST        const
#define _SIZET        size_t

#endif /* !_CONST_H_ */