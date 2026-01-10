
#ifndef _STDIO_H_
#define _STDIO_H_ 1

#include <stdarg.h>
#include "sys/stdint.h"

void 	putchar(char ch); // libc/stdio/putchar.c
void 	puts(const char* str); // libc/stdio/puts.c
void 	vprintf(const char* str, va_list arg); // libc/stdio/vprintf.c
void 	printf(const char* fmt, ...); // libc/stdio/printf.c

// libc/stdio/misc.c
void 	set_color(uint8_t color); 
uint8_t get_color();
void 		gotoindex(uint16_t index);
void 		gotoxy(uint8_t x, uint8_t y);
uint16_t get_index();

#endif // !_STDIO_H_