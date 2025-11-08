
/*
* printf.c -- CHB printf function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdio.h"

#define PRINTF_LIMIT 1024

PRIVATE char _buff[PRINTF_LIMIT];

int printf(_CONST char *fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    int ret = vsprintf(_buff, fmt, args);
    va_end(args);
	
	/* print buffer in screen */
	puts(_buff);
	
    return ret;
}
