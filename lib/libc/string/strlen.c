
/*
* strlen.c -- strlen function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

size_t
strlen(const char* s) {
   const char* str = s;
   size_t index;
   while (*str != '\0') {
      *str++;
      index++;
   }
   return index;
}