
/*
* lower.c -- tolower and islower functions
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/ctype.h>

char
tolower(int c) {
   return isupper(c) ? (c - 'A' + 'a') : c;
}

int
islower(int c) {
   return c >= 'a' && c <= 'z';
}