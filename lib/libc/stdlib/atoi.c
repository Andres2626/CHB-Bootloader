
/*
* atoi.c -- CHB atoi function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdlib.h"

int atoi(char *ptr) 
{
    int number = 0;
    int sign = 1;
    
    /* check if number is negative */
    if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
	else if (*ptr == '+')
        ptr++;

    while (*ptr >= '0' && *ptr <= '9') {
        number = number * 10 + (*ptr - '0');
		ptr++;
    }

    return sign * number;
}