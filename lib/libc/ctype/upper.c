
/*
* upper.c -- toupper and isupper functions
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/ctype.h"

char
toupper(int c) {
   return islower(c) ? (c - 'a' + 'A') : c;
}

int
isupper(int c) {
   return c >= 'A' && c <= 'Z';
}