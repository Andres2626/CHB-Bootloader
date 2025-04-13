
/*
* strtok.c -- strtok function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/string.h>

static char* ___strtok = NULL;

char*
strtok(char* s1, char const* s2) {
   char *sbegin, *send;
   sbegin = s1 ? s1 : ___strtok;
   if (!sbegin) {
      return NULL;
   }

   sbegin += strspn(sbegin, s2);
   if (*sbegin == '\0') {
      ___strtok = NULL;
      return NULL;
   }

   send = strpbrk(sbegin, s2);
   if (send && *send != '\0') {
      *send++ = '\0';
   }
   ___strtok = send;
   return sbegin;
}