
/*
* memcmp.c -- memcmp function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

int
memcmp(const void* s1, const void* s2, size_t n) {
   const uint8_t* ptr1 = (const uint8_t*)s1;
   const uint8_t* ptr2 = (const uint8_t*)s2;

   for (uint16_t i = 0; i < n; i++) {
      if (ptr1[i] != ptr2[i]) {
         return 1;
      }
   }
   return 0;
}
