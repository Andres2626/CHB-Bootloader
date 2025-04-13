
/*
* strncmp.c -- strncmp function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

int
strncmp(const char* s1, const char* s2, size_t n) {
   while (n && *s1 && (*s1 == *s2)) {
      ++s1;
      ++s2;
      --n;
   }

   if (n == 0) {
      return 0;
   } else {
      return (*(unsigned char*)s1 - *(unsigned char*)s2);
   }
}