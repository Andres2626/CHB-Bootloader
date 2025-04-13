
/*
* strspn.c -- strspn function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

size_t
strspn(char const* s1, char const* s2) {
   const char* p;
   const char* a;
   size_t count = 0;

   for (p = s1; *p != '\0'; ++p) {
      for (a = s2; *a != '\0'; ++a) {
         if (*p == *a) {
            break;
         }
      }
      if (*a == '\0') {
         return count;
      }
      ++count;
   }

   return count;
}