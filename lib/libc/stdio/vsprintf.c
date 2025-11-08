/*
* vsprintf.c -- CHB vsprintf function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdio.h"
#include "lib/libc/stdlib.h"

#define STATE_NORMAL 0
#define STATE_FORMAT 1
#define STATE_LONG 2
#define STATE_LLONG 3
#define BUFFSZ 32

void copy_internal(_CONST char *src, char **dst)
{
	while (*src) {
        **dst = *src++;
        (*dst)++;
    }
}

int vsprintf(char *s, _CONST char *fmt, va_list arg) 
{
	char *begin = s;
    *s = 0;

    int state = STATE_NORMAL;
    while (*fmt) {
        if (state == STATE_NORMAL) {
            if (*fmt == '%')
                state = STATE_FORMAT;
            else
                *s++ = *fmt;
			fmt++;
        }
		else if (state >= STATE_NORMAL) {
			if (*fmt == 'l') {
                if (state == STATE_FORMAT)
                    state = STATE_LONG;
                else if (state == STATE_LONG)
                    state = STATE_LLONG;
                fmt++;
                continue;
		    }
			
			char buff[BUFFSZ];
			
			i64t n = 0;
			
            switch (*fmt) {
                case 'c': {
                    char ch = va_arg(arg, int);
                    *s++ = ch;
                    break;
                }
                case 's': {
                    const char *str = va_arg(arg, const char*);
                    copy_internal(str, &s);
                    break;
                }
                case 'i':
                case 'd': {
					switch (state) {
					case STATE_LLONG:
					    n = va_arg(arg, i64t);
						itoa64(buff, 10, n);
					    break;
					case STATE_LONG:
					    n = va_arg(arg, i32t);
						itoa64(buff, 10, (i64t)n);
					    break;
					default:
					    n = va_arg(arg, int);
						itoa(buff, 10, n);
					    break;
					}
					
					copy_internal(buff, &s);
                    break;
                }
                case 'u': {
                    switch (state) {
					case STATE_LLONG:
					    n = va_arg(arg, u64t);
						utoa64(buff, 10, n);
					    break;
					case STATE_LONG:
					    n = va_arg(arg, u32t);
						utoa64(buff, 10, (u64t)n);
					    break;
					default:
					    n = va_arg(arg, unsigned int);
						itoa(buff, 10, n);
					    break;
					}
                    copy_internal(buff, &s);
                    break;
                }
                case 'X':
                case 'x': {
                    switch (state) {
					case STATE_LLONG:
					    n = va_arg(arg, u64t);
						utoa64(buff, 16, n);
					    break;
					case STATE_LONG:
					    n = va_arg(arg, u32t);
						utoa64(buff, 16, (u64t)n);
					    break;
					default:
					    n = va_arg(arg, unsigned int);
						itoa(buff, 16, n);
					    break;
					}
                    copy_internal(buff, &s);
                    break;
                }
                case 'o': {
                    switch (state) {
					case STATE_LLONG:
					    n = va_arg(arg, u64t);
						utoa64(buff, 8, n);
					    break;
					case STATE_LONG:
					    n = va_arg(arg, u32t);
						utoa64(buff, 8, (u64t)n);
					    break;
					default:
					    n = va_arg(arg, unsigned int);
						itoa(buff, 8, n);
					    break;
					}
					copy_internal(buff, &s);
                    break;
                }
				case 'p': {
					_VOIDSTAR ptr = va_arg(arg, _VOIDSTAR);
					uptrt addr = (uptrt)ptr;
					*s++ = '0';
					*s++ = 'x';
					utoa64(buff, 16, (u64t)addr);
					copy_internal(buff, &s);
					break;
                }
				case '%': {
                    *s++ = '%';
                    break;
                }
				default: {
					*s++ = '%';
                    *s++ = *fmt;
				}
            }
			fmt++;
            state = STATE_NORMAL;
		}
    }
    *s = 0;
    return (s - begin);
}