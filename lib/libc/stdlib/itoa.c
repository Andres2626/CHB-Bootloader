
/*
* itoa.c -- CHB itoa function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdlib.h>

char*
itoa(char* buf, int base, int n) {
   char* rc;
   char* ptr;
   char* low;

   if (base < 2 || base > 36) {
      *buf = '\0';
      return buf;
   }
   rc = ptr = buf;

   if (n < 0 && base == 10) {
      *ptr++ = '-';
   }

   low = ptr;
   do {
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + n % base];
      n /= base;
   } while (n);

   *ptr-- = '\0';

   while (low < ptr) {
      char tmp = *low;
      *low++ = *ptr;
      *ptr-- = tmp;
   }
   return rc;
}