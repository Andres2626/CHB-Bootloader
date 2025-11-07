
/*
* strncmp.c -- strncmp function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

int strncmp(_CONST char* s1, _CONST char* s2, size_t n) 
{
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        --n;
    }

    if (!n)
        return 0;
	
    return (*(unsigned char*)s1 - *(unsigned char*)s2);
}