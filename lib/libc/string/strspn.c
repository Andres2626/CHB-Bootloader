
/*
* strspn.c -- strspn function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

size_t strspn(_CONST char *s1, _CONST char *s2)
{
    size_t count = 0;
    for (; *s1; s1++) {
        const char *p = s2;
        while (*p && *p != *s1)
            p++;
        if (!*p)
            break;
		
        count++;
    }
    return count;
}