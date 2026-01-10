
/*
 * types.h -- CHB types
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _TYPES_H_
#define _TYPES_H_ 1

/* boolean types */
#define bool  _Bool
#define true  1
#define false 0

/* integer types */
typedef signed char        i8t;
typedef signed short       i16t;
typedef signed int         i32t;
typedef signed long long   i64t;

/* unsigned integer types */
typedef unsigned char      u8t;
typedef unsigned short     u16t;
typedef unsigned int       u32t;
typedef unsigned long long u64t;

typedef u32t uptrt;

#endif /* !_TYPES_H_ */