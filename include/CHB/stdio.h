
#ifndef _STDIO_H_
#define _STDIO_H_ 1

#include <stdarg.h>
#include <CHB/types.h>

void clrscr(void);

void putchar(int c);
int getchar(void);

void puts(const char *s);
int vprintf(const char *fmt, va_list arg);
int vsprintf(char *s, const char *fmt, va_list arg);
int printf(const char *fmt, ...);
int sprintf(char* s, const char *fmt, ...);

#endif /* !_STDIO_H_ */