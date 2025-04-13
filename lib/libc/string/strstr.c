
/*
* strstr.c -- strstr function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

char*
strstr(const char* s1, const char* s2) {
   size_t len = strlen(s2);

   while (*s1) {
      if (!memcmp((char*)s1, s2, len)) {
         return (char*)s1;
      }

      s1++;
   }

   return NULL;
}