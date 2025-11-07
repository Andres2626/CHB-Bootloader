
/*
* itoa64.c -- CHB itoa64 function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdlib.h"

PRIVATE void div_internal(u64t n, u32t base, u64t *q, u64t *r) 
{
	u64t _q = 0;
	u64t _r = 0;
	
    for (int i = 63; i >= 0; i--) {
        _r = (_r << 1) | ((n >> i) & 1);
        if (_r >= base) {
            _r -= base;
            _q |= ((u64t)1 << i);
        }
    }
    
	*q = _q;
	*r = _r;
}

PRIVATE u64t u64_div(u64t n, u32t base) 
{
	u64t q = 0;
	u64t r = 0;
	div_internal(n, base, &q, &r);
	return q;
}

PRIVATE u64t u64_mod(u64t n, u32t base) {
	u64t q = 0;
	u64t r = 0;
    div_internal(n, base, &q, &r);
    return r;
}

char *itoa64(char *buf, int base, i64t n) 
{
    if (base < 2 || base > 36) {
        *buf = 0;
        return buf;
    }

    char *ptr = buf;
    char *start;
    u64t num;

    if ((int64_t)n < 0 && base == 10) {
        *ptr++ = '-';
        num = (uint64_t)(-(n + 1)) + 1;
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