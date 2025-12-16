
/*
 * stdlib.h -- stdlib prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_ 1

#include "types.h"
#include "const.h"

PROTOTYPE(u64t u64_div, (u64t n, u32t base));
PROTOTYPE(u64t u64_mod, (u64t n, u32t base));
PROTOTYPE(int atoi, (char *ptr));
PROTOTYPE(char *utoa64, (char *buf, int base, u64t n));
PROTOTYPE(char *itoa64, (char *buf, int base, i64t n));
PROTOTYPE(char *itoa, (char *buf, int base, int n));

#endif /* !_STDLIB_H_ */