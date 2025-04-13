
/*
* vsprintf.c -- CHB vsprintf function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdio.h>

extern char* itoa(char* buf, int base, int n);

int
vsprintf(char* s, const char* fmt, va_list arg) {
   *s = '\0';

   int state = 0;
   while (*fmt) {
      if (state == 0) {
         if (*fmt == '%') {
            state = 1;
         } else {
            *s = *fmt;
         }
      } else if (state == 1) {
         switch (*fmt) {
            case 'c': {
               *s-- = '\0';
               char ch = va_arg(arg, int);
               *s = ch;
               break;
            }
            case 's': {
               *s-- = '\0';
               const char* string = va_arg(arg, const char*);
               while (*string) {
                  *s = *string;
                  string++;
                  s++;
               }
               *s-- = '\0';
               break;
            }
            case 'i':
            case 'd': {
               *s-- = '\0';
               int number = va_arg(arg, int);
               char* buffer;
               itoa(buffer, 10, number);
               while (*buffer) {
                  *s = *buffer;
                  buffer++;
                  s++;
               }
               break;
            }
            case 'u': {
               *s-- = '\0';
               int number = va_arg(arg, int);
               if (number < 0) {
                  number = (-number);
               }
               char* buffer;
               itoa(buffer, 10, number);
               while (*buffer) {
                  *s = *buffer;
                  buffer++;
                  s++;
               }
               break;
            }
            case 'p': {
               *s-- = '\0';
               *s++ = '0';
               *s++ = 'x';
               void* p = va_arg(arg, void*);
               char* buffer;
               itoa(buffer, 16, (int)p);
               while (*buffer) {
                  *s = *buffer;
                  buffer++;
                  s++;
               }
               break;
            }
            case 'X':
            case 'x': {
               *s-- = '\0';
               int number = va_arg(arg, int);
               char* buffer;
               itoa(buffer, 16, number);
               while (*buffer) {
                  *s = *buffer;
                  buffer++;
                  s++;
               }
               break;
            }
            case 'o': {
               *s-- = '\0';
               int number = va_arg(arg, int);
               char* buffer;
               itoa(buffer, 8, number);
               while (*buffer) {
                  *s = *buffer;
                  buffer++;
                  s++;
               }
               break;
            }
         }

         state = 0;
      }
      s++;
      fmt++;
   }

   return 0; /* TODO: not implemented */
}