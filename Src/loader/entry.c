
/* entry.c -- C part of the loader */

#include <CHB/types.h>
#include <CHB/video.h>
#include <CHB/port.h>

#include <stdarg.h>

#define VIDEO_TEXT_MEMORY (uint8_t*)0xb8000

uint8_t video_mode;
uint8_t video_cursor_start, video_cursor_end;
uint16_t video_color;
uint16_t video_cursor_index;
uint8_t *video_mm;

/* video functions */
void video_initialize(uint8_t mode);

/* cursor functions */
void video_cursor_enable(uint8_t start, uint8_t end);
void video_cursor_disable(void);
void video_cursor_gotoindex(uint16_t index);
void video_cursor_gotoxy(uint16_t x, uint16_t y);
uint16_t video_cursor_getindex(void);
void video_cursor_set_color(uint8_t background, uint8_t foreground); /* implement graphic modes */
uint16_t video_cursor_get_color(void);

/* video mm write */
void video_putchar(char c);

/* video misc functions */
void video_cls(void);
void video_scroll(void);

/* libc functions */
void putchar(int c);
void puts(const char *s);
int vprintf(const char *fmt, va_list arg);
int printf(const char *fmt, ...);
char* itoa(char *buf, int base, int n);

/* video cursor colors */
void 
entry(unsigned int param_address, unsigned int size) {
	
	video_initialize(0x3);
	
	printf("Hello World! %i", 1234);
	
	/* infinite loop */
	for (;;);
}

void
video_initialize(uint8_t mode) {
	/* set BIOS video mode */
	video_mode = video_set_mode(mode);
	
	/* setup cursor */
	video_cursor_enable(14, 15);
	
	/* set video MM loc */
	video_mm = VIDEO_TEXT_MEMORY;
	
	/* background: black and foreground: gray */
	video_cursor_set_color(0x0, 0x7);
}

void 
video_cursor_enable(uint8_t start, uint8_t end) {
	video_cursor_start = start;
	video_cursor_end = end;
	
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);
	port_outb(0x3D4, 0x0B);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
}

void 
video_cursor_disable(void) {
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D4, 0x20);
}

void 
video_cursor_gotoindex(uint16_t index) {
	video_cursor_index = index;
	port_outb(0x3D4, 0x0F);
	port_outb(0x3D5, (uint8_t) (index & 0xFF));
	port_outb(0x3D4, 0x0E);
	port_outb(0x3D5, (uint8_t) ((index >> 8) & 0xFF));
}

void 
video_cursor_gotoxy(uint16_t x, uint16_t y) {
	video_cursor_index = y * 25 + x;
	video_cursor_gotoindex(video_cursor_index);
}

uint16_t 
video_cursor_getindex(void) {
	return video_cursor_index;
}

void 
video_putchar(char c) {
	uint16_t index = video_cursor_getindex();

	switch (c) {
	case '\0':
		*(video_mm + index * 2) = 0x0;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
		break;
	case '\'':
		*(video_mm + index * 2) = 0x27;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
		break;
	case '\"':
		*(video_mm + index * 2) = 0x22;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
		break;
	case '\?':
		*(video_mm + index * 2) = 0x3f;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
		break;
	case '\\':
		*(video_mm + index * 2) = 0x5c;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
		break;
	case '\b':
		*(video_mm + index * 2) = 0x00;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index--;
		break;
	case '\f':
		index += 80;
		break;
	case '\t':
		index = (index + 8) & ~(8 - 1);
		break;
	case '\v':
		index += 80;
		break;
	case 10:
		index -= index % 80 - 80;
		break;
	case 13:
		index -= index % 80;
		break;
	default:
		{
			*(video_mm + index * 2) = c;
			*(video_mm + index * 2 + 1) = video_cursor_get_color();
			index++;
			break;
		}
	}

	video_cursor_gotoindex(index);
	video_scroll();
}

void 
video_cursor_set_color(uint8_t background, uint8_t foreground) {
	video_color = (background << 4) | (foreground & 0x0F);
}

uint16_t 
video_cursor_get_color(void) {
	return video_color;
}

void 
video_cls(void) {
	video_cursor_gotoxy(0, 0);
	
	for (int i = 0; i < (80 * 25); i++) {
		video_putchar(' ');
	}
	
	video_cursor_gotoxy(0, 0);
}

void 
video_scroll(void) {
	/* convert index to x, y coordinates */
	uint8_t cursor_x = video_cursor_getindex() % 25;
	uint8_t cursor_y = (video_cursor_getindex() - cursor_x) / 25;
	
	//video_cursor_set_color(0x0, 0x7);
	
	if (cursor_x >= 25) {
        for (int i = 0; i < 24*80; i++) {
            video_mm[i] = video_mm[i+80];
        }

        for (int i = 24*80; i < 25*80; i++) {
            video_mm[i] = ' ';
        }

        cursor_y = 24;
	}
	
	video_cursor_gotoxy(cursor_x, cursor_y);
}

void
putchar(int c) {
	video_putchar(c);
}

void
puts(const char *s) {
	while (*s) {
		putchar(*s);
		s++;
	}
}

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

int 
printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int ret = vprintf(fmt, args);

	va_end(args);
	
	return ret;
}

char* 
itoa(char *buf, int base, int n) {
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