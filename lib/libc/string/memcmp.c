
/*
* memcmp.c -- memcmp function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

int memcmp(_CONST void *s1, _CONST void *s2, size_t n) 
{
    _CONST u8t *_s1 = (_CONST u8t*)s1;
    _CONST u8t *_s2 = (_CONST u8t*)s2;

    while (n--) {
        if (*_s1 != *_s2)
            return (*_s1 < *_s2) ? -1 : 1;
		
		_s1++;
		_s2++;
    }

    return 0;
}
