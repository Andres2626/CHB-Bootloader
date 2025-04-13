
/*
* memset.c -- memset function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

void*
memset(void* b, int c, size_t n) {
   unsigned char* p = b;
   while (n > 0) {
      *p = c;
      p++;
      n--;
   }
   return b;
}
