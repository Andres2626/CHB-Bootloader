
/*
* puts.c -- CHB puts function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdio.h"

void puts(_CONST char *s) 
{
    while (*s) {
        putchar(*s);
        s++;
    }
}