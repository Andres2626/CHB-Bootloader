
/*
 * types.h -- CHB types
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _TYPES_H_
#define _TYPES_H_ 1

#include <stdint.h>

#if defined (bool) 
#undef bool
#endif

#if defined (true)
#undef true
#endif

#if defined (false)
#undef false
#endif

/* boolean types */
#define bool  _Bool
#define true  1
#define false 0

/* integer types */
typedef int8_t i8t;
typedef int16_t i16t;
typedef int32_t i32t;
typedef int64_t i64t;

/* unsigned integer types */
typedef uint8_t u8t;
typedef uint16_t u16t;
typedef uint32_t u32t;
typedef uint64_t u64t;

typedef u32t uptrt;

#endif /* !_TYPES_H_ */