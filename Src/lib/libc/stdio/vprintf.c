
#include <CHB/stdio.h>

extern char* itoa(char *buf, int base, int n);

int
vprintf(const char *fmt, va_list arg) {
	int state = 0;
	while (*fmt) {
		if (state == 0) {
			if (*fmt == '%') 
				state = 1;
			else
				putchar(*fmt);
		}
		else if (state == 1) {
			switch (*fmt) {
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
				itoa(buffer, 10, number);
				puts(buffer);
				break;
			}
			case 'u': {
				int number = va_arg(arg, int);

				if (number < 0) {
					number = (-number);
				}

				char buffer[64];
				itoa(buffer, 10, number);
				puts(buffer);
				break;
			}
			case 'p': {
				putchar('0');
				putchar('x');
				void* p = va_arg(arg, void*);
				char buffer[32];
				itoa(buffer, 16, (int)p);
				puts(buffer);
				break;
			}
			case 'X':
			case 'x': {
				int number = va_arg(arg, int);
				char buffer[64];
				itoa(buffer, 16, number);
				puts(buffer);
				break;
			}
			case 'o': {
				int number = va_arg(arg, int);
				char buffer[64];
				itoa(buffer, 8, number);
				puts(buffer);
				break;
			}
			}

			state = 0;
		}
		fmt++;
	}
	
	return 0; /* TODO: not implemented */
}
