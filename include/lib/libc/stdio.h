
/*
 * stdio.h -- stdio prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STDIO_H_
#define _STDIO_H_ 1

#include "types.h"
#include "const.h"

/* compiler dependent */
#include <stdarg.h>

PROTOTYPE(void clrscr, (void));
PROTOTYPE(void putchar, (int c));
PROTOTYPE(int getchar, (void));
PROTOTYPE(void puts, (_CONST char *s));
PROTOTYPE(int vprintf, (_CONST char* fmt, va_list arg));
PROTOTYPE(int vsprintf, (char *s, _CONST char* fmt, va_list arg));
PROTOTYPE(int printf, (_CONST char *fmt, ...));
PROTOTYPE(int sprintf, (char *s, _CONST char *fmt, ...));

#endif /* !_STDIO_H_ */