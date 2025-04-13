
/*
* strchr.c -- strchr function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

char*
strchr(const char* s, int c) {
   for (; *s != (char)c; ++s) {
      if (*s == '\0') {
         return NULL;
      }
   }
   return (char*)s;
}
