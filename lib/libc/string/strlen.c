
/*
* strlen.c -- strlen function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"

size_t strlen(_CONST char *s) 
{
    _CONST char* str = s;
    size_t index = 0;
   
    while (*str++) {
	    index++;
    }
   
    return index;
}