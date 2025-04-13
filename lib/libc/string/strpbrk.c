
/*
* strpbrk.c -- strpbrk function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

char*
strpbrk(char const* s1, char const* s2) {
   const char* sc1;
   const char* sc2;
   for (sc1 = s1; *sc1 != '\0'; sc1++) {
      for (sc2 = s2; *sc2 != '\0'; sc2++) {
         if (*sc1 == *sc2) {
            return (char*)sc1;
         }
      }
   }
   return NULL;
}