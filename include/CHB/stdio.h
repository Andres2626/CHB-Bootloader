
/*
 * stdio.h -- stdio prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _STDIO_H_
#define _STDIO_H_ 1

#include <CHB/types.h>
#include <stdarg.h>

void clrscr(void);

void putchar(int c);
int getchar(void);

void puts(const char* s);
int vprintf(const char* fmt, va_list arg);
int vsprintf(char* s, const char* fmt, va_list arg);
int printf(const char* fmt, ...);
int sprintf(char* s, const char* fmt, ...);

#endif /* !_STDIO_H_ */