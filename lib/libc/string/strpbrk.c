
/*
* strpbrk.c -- strpbrk function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

char *strpbrk(_CONST char *s1, _CONST char *s2) 
{
    while (*s1) {
		const char *p = s2;
        while (*p) {
            if (*s1 == *p)
                return (char*)s1;
            p++;
        }
        s1++;
    }
	
    return NULL;
}