
/*
* itoa64.c -- CHB itoa64 function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdlib.h"

char *itoa64(char *buf, int base, i64t n) 
{
    if (base < 2 || base > 36) {
        *buf = 0;
        return buf;
    }

    char *ptr = buf;
    char *start;
    u64t num;

    if ((i64t)n < 0 && base == 10) {
        *ptr++ = '-';
        num = (u64t)(-(n + 1)) + 1;
    } 
	else
        num = n;

    start = ptr;

    do {
        u64t digit = u64_mod(num, base);
        *ptr++ = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
        num = u64_div(num, base);
    } while (num);

    /* NULL terminated string */
    *ptr-- = 0;

    while (start < ptr) {
        char tmp = *start;
        *start++ = *ptr;
        *ptr-- = tmp;
    }

    return buf;
}