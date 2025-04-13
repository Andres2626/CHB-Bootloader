
/*
* memcpy.c -- memcpy function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

void*
memcpy(void* s1, const void* s2, size_t n) {
   char* cdest = (char*)s1;
   char* csrc = (char*)s2;
   size_t index = 0;

   while (index < n) {
      *cdest = *csrc;
      cdest++;
      csrc++;
      index++;
   }

   return cdest;
}
