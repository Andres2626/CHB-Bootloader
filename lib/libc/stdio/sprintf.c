
/*
* sprintf.c -- CHB sprintf function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdio.h>

int
sprintf(char* s, const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);

   int ret = vsprintf(s, fmt, args);

   va_end(args);

   return ret;
}