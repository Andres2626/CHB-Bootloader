
/*
* memcpy.c -- memcpy function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

void *memcpy(void *s1, _CONST void *s2, size_t n) 
{
    u8t *_s1 = s1;
	_CONST u8t *_s2 = s2;

    while (n--) {
	    *_s1++ = *_s2++;
    }

    return s1;
}
