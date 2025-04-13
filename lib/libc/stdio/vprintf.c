
/*
* vprintf.c -- CHB vprintf function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdio.h>

/* TODO: implement %f and ftoa */

extern char* itoa(char* buf, int base, int n);

int
vprintf(const char* fmt, va_list arg) {
   int state = 0;
   while (*fmt) {
      if (state == 0) {
         if (*fmt == '%') {
            state = 1;
         } else {
            putchar(*fmt);
         }
      } else if (state == 1) {
         switch (*fmt) {
            case 'c': { // char
               char ch = va_arg(arg, int);
               putchar(ch);
               break;
            }
            case 's': { // char*
               const char* string = va_arg(arg, const char*);
               while (*string) {
                  putchar(*string); /* write to video memory */
                  string++;
               }
               break;
            }
            case 'i': // integer
            case 'd': {
               int number = va_arg(arg, int);
               char buffer[64];
               itoa(buffer, 10, number);
               puts(buffer);
               break;
            }
            case 'u': { //unsigned int
               int number = va_arg(arg, int);

               if (number < 0) {
                  number = (-number);
               }

               char buffer[64];
               itoa(buffer, 10, number);
               puts(buffer);
               break;
            }
            case 'p': { // pointer
               putchar('0');
               putchar('x');
               void* p = va_arg(arg, void*);
               char buffer[32];
               itoa(buffer, 16, (int)p);
               puts(buffer);
               break;
            }
            case 'X':
            case 'x': { // hexadecimal
               int number = va_arg(arg, int);
               char buffer[64];
               itoa(buffer, 16, number);
               puts(buffer);
               break;
            }
            case 'o': { // octal
               int number = va_arg(arg, int);
               char buffer[64];
               itoa(buffer, 8, number);
               puts(buffer);
               break;
            }
            case 'f': {
               // TODO: implement this
               break;
            }
         }

         state = 0;
      }
      fmt++;
   }

   return 0; /* TODO: not implemented */
}
