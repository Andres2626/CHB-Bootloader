
/*
* puts.c -- CHB puts function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdio.h>

void
puts(const char* s) {
   while (*s != '\0') {
      putchar(*s);
      s++;
   }
}