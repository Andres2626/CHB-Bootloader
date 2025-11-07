
/*
* utoa64.c -- utoa64 itoa function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdlib.h"

char *utoa64(char *buf, int base, u64t n) 
{
    return itoa64(buf, base, (i64t)n);
}