
/*
* strchr.c -- strchr function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

char *strchr(_CONST char *s, int c) 
{
	while (*s != (char)c) {
		if (!*s)
            return NULL;
		
		s++;
	}
    return (char*)s;
}
