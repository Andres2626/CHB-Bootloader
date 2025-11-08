
/*
 * string.h -- string prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STRING_H_
#define _STRING_H_ 1

#include "types.h"
#include "const.h"

typedef unsigned int size_t;
#define NULL ((void*)0)

PROTOTYPE(int memcmp, (_CONST _VOIDSTAR s1, _CONST _VOIDSTAR s2, size_t n));
PROTOTYPE(void *memcpy, (void *s1, _CONST _VOIDSTAR s2, size_t n));
PROTOTYPE(void *memset, (void *b, int c, size_t n));
PROTOTYPE(int strncmp, (_CONST char *s1, _CONST char *s2, size_t n));
PROTOTYPE(size_t strlen, (_CONST char *s));
PROTOTYPE(char *strstr, (_CONST char *s1, _CONST char *s2));
PROTOTYPE(char *strchr, (_CONST char *s, int c));
PROTOTYPE(size_t strspn, (char _CONST *s1, char _CONST *s2));
PROTOTYPE(char *strpbrk, (char _CONST *s1, char _CONST *s2));
PROTOTYPE(char *strtok, (char *s1, char _CONST *s2));
PROTOTYPE(char *strerror, (int errno));

#endif /* !_STRING_H_ */