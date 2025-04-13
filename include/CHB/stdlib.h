
/*
 * stdlib.h -- stdlib prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_ 1

#include <CHB/types.h>

int atoi(char* ptr);
char* itoa(char* buf, int base, int n);

#endif /* !_STDLIB_H_ */