
#include "sys\stdio.h"
#include "sys\stdlib.h"

#define PRINTF_NO_FORMAT 0
#define PRINTF_FORMATED 1

void vprintf(const char* str, va_list arg) {
	//TODO: Implement %x, %X, %f
	int state = 0;
	while (*str) {
		if (state == PRINTF_NO_FORMAT) {
			if (*str == '%') 
				state = 1;
			else
				putchar(*str);
		}
		else if (state == PRINTF_FORMATED) {
			switch (*str) {
			case 'c': {
				char ch = va_arg(arg, int);
				putchar(ch);
				break;
			}
			case 's': {
				const char* string = va_arg(arg, const char*);
				while (*string)
				{
					putchar(*string);
					string++;
				}
				break;
			}
			case 'i':
			case 'd': {
				int number = va_arg(arg, int);
				char buffer[64];
				itoa(number, buffer, 10);
				puts(buffer);
				break;
			}
			case 'u': {
				int number = va_arg(arg, int);

				if (number < 0)
				{
					number = (-number);
				}

				char buffer[64];
				itoa(number, buffer, 10);
				puts(buffer);
				break;
			}
			case 'p': {
				putchar('0');
				putchar('x');

				void* p = va_arg(arg, void*);

				char buffer[32];
				itoa((int)p, buffer, 16);
				puts(buffer);
				break;
			}
			case 'X':
			case 'x': {
				int number = va_arg(arg, int);
				char buffer[64];
				itoa(number, buffer, 16);
				puts(buffer);
				break;
			}
			case 'o': {
				int number = va_arg(arg, int);
				char buffer[64];
				itoa(number, buffer, 8);
				puts(buffer);
				break;
			}
			}

			state = PRINTF_NO_FORMAT;
		}

		str++;
	}
}