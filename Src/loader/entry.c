
/* entry.c -- C part of the loader */

#include <CHB/types.h>
#include <CHB/video.h>
#include <CHB/port.h>

#include <stdarg.h>

#define VIDEO_TEXT_MEMORY (uint8_t*)0xb8000

typedef unsigned int size_t;
#define NULL ((void*)0)

uint8_t video_mode = 0;
uint8_t video_cursor_start = 0, video_cursor_end = 0;
uint16_t video_color = 0;
uint16_t video_cursor_index = 0;
uint8_t *video_mm;

/* video functions */
void video_initialize(uint8_t mode);

/* cursor functions */
void video_cursor_enable(uint8_t start, uint8_t end);
void video_cursor_disable(void);
void video_cursor_gotoindex(uint16_t index);
void video_cursor_gotoxy(uint8_t x, uint8_t y);
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
int vsprintf(char *s, const char *fmt, va_list arg);
int printf(const char *fmt, ...);
int sprintf(char* s, const char *fmt, ...);
char* itoa(char *buf, int base, int n);
int getchar(void);

void* memcpy(void *s1, const void *s2, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
char* strstr(const char *s1, const char *s2);
char* strchr(const char *s, int c);
size_t strspn(char const *s1, char const *s2);
char* strpbrk(char const *s1, char const *s2);
char* strtok(char *s1, char const *s2);

char** terminal_parse_argv(char* cmd);
uint8_t terminal_parse_argc(char* cmd);
void terminal_execute_command(int argc, char** argv);

/* video cursor colors */
void 
entry(unsigned int param_address, unsigned int size) {
	int count = 0, cursor = 0;
	uint8_t argc = 0;
	char buffer[64];
	char** argv;
	
	video_initialize(0x3);
	printf("Welcome to CHB\n");
	printf("Executing shell...\n");
	
	printf("> ");
	while (1) {
		int key = getchar();
		if (key != '\r') {
			if (key == '\b')
				cursor--;
			else
				cursor++;
			
			if (cursor < 0)
				cursor = -1;
			
			/* key is a backspace and cursor is -1? */
			if (key == '\b' && cursor != -1) {
				putchar('\b');
				buffer[count] = '\b';
				count--;
			}
			
			/* is key printable? */
			if (cursor >= 0 && key >= 0x20) {
				putchar(key);
				buffer[count] = key;
				count++;
			}
		}
		else {
			printf("\n");
			char *arg = buffer;
			
			/* get argument data */
			argc = terminal_parse_argc(arg);
			argv = terminal_parse_argv(arg);
			
			terminal_execute_command(argc, argv);
			
			/* fill buffer with ceroes */
			for (int i = 0; i < 64; i++) {
				buffer[i] = '\0';
			}
			
			printf("\n> ");
			cursor = 0;
			count = 0;
		}
	}
}

char**
terminal_parse_argv(char* cmd) {
	char **argv;
	int count = 0;
	
	/* separate input considering spaces */
	char *token = strtok(cmd, " ");
	while (token != NULL) {
		argv[count] = token;
		token = strtok(NULL, " ");
		count++;
	}
	return argv;
}

uint8_t
terminal_parse_argc(char* cmd) {
	uint8_t count = 0;
	
	/* check how many spaces there are */
	while((cmd = strchr(cmd, 0x20)) != NULL) {                                                                                                                       
		count++;                                                                                                             
		cmd++;
	}
	return count;
}

void 
terminal_execute_command(int argc, char **argv) {
	if (argv[0][0] == '\0')
		return; /* NULL command */
	
	if (!strncmp(argv[0], "help\0", 5))
		command_help(argc, argv);
	else
		printf("%s command not found. Type help for more info.\n", argv[0]);
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
	
	/* clean display */
	video_cls();
}

void 
video_cursor_enable(uint8_t start, uint8_t end) {	
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);
	port_outb(0x3D4, 0x0B);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
	
	video_cursor_start = start;
	video_cursor_end = end;
}

void 
video_cursor_disable(void) {
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D4, 0x20);
}

void 
video_cursor_gotoindex(uint16_t index) {
	port_outb(0x3D4, 0x0F);
	port_outb(0x3D5, (uint8_t) (index & 0xFF));
	port_outb(0x3D4, 0x0E);
	port_outb(0x3D5, (uint8_t) ((index >> 8) & 0xFF));
	video_cursor_index = index;
}

void 
video_cursor_gotoxy(uint8_t x, uint8_t y) {
	video_cursor_gotoindex(y * 80 + x);
}

uint16_t 
video_cursor_getindex(void) {
	return video_cursor_index;
}

void 
video_putchar(char c) {
	uint16_t index = video_cursor_getindex();
	
	/* backspace */
	if (c == '\b') {
		index--;
		*(video_mm + index * 2) = 0x0;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
	}
	/* vertical tab */
	else if (c == '\f' || c == '\v') {
		index += 80;
	}
	/* horizontal tab */
	else if (c =='\t') {
		index = (index + 8) & ~(8 - 1);
	}
	/* newline */
	else if (c == '\n') {
		index -= index % 80 - 80;
	}
	/* carriage return */
	else if (c == '\r') {
		index -= index % 80;
	}
	/* any printable character */
	else if (c >= 0x20) {
		*(video_mm + index * 2) = c;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
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
	uint8_t cursor_x = video_cursor_getindex() % 80;
	uint8_t cursor_y = (video_cursor_getindex() - cursor_x) / 80;
	uint16_t* loc = (uint16_t*)0xb8000;
	video_cursor_set_color(0x0, 0x7);
	
	uint8_t attributeByte = video_cursor_get_color();
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);
	
	if (cursor_y >= 25) {
        for (int i = 0; i < 24*80; i++) {
            loc[i] = loc[i+80];
        }

        for (int i = 24*80; i < 25*80; i++) {
            loc[i] = blank;
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
vsprintf(char *s, const char *fmt, va_list arg) {
	*s = '\0';
	
	int state = 0;
	while (*fmt) {
		if (state == 0) {
			if (*fmt == '%') 
				state = 1;
			else
				*s = *fmt;
		}
		else if (state == 1) {
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
				char *buffer;
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
				char *buffer;
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
				char *buffer;
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
				char *buffer;
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
				char *buffer;
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

int
sprintf(char* s, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int ret = vsprintf(s, fmt, args);

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

int 
getchar(void) {
	int key = keyboard_get_key_status() & 0xFF;
	return key;
}

/* string functions */
void* 
memcpy(void *s1, const void *s2, size_t n) {
	/*set the arrays (char[])*/
	char* cdest = (char*)s1;
	char* csrc = (char*)s2;

	int index = 0;
	while (index < n) {
		*cdest = *csrc;
		cdest++;
		csrc++;
		index++;
	}
	
	return cdest;
}

int 
memcmp(const void* s1, const void* s2, size_t n) {
	const uint8_t *ptr1 = (const uint8_t*)s1;
    const uint8_t *ptr2 = (const uint8_t*)s2;
	
    for (uint16_t i = 0; i < n; i++) {
        if (ptr1[i] != ptr2[i]) {
            return 1;	
		}
	}
    return 0;	
}
    

int 
strncmp(const char *s1, const char *s2, size_t n) {	
	while ( n && *s1 && ( *s1 == *s2 ) ) {
        ++s1;
        ++s2;
        --n;
    }
	
    if ( n == 0 ) {
        return 0;
    }
    else {
        return (*(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

size_t 
strlen(const char *s) {
	size_t index;
	while (*s != '\0') {
		*s++;
		index++;
	}
	
	return index;
}

char* 
strchr(const char *s, int c) {	
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

char* 
strstr(const char *s1, const char *s2) {	
	size_t len = strlen(s2) + 1;
	
	while (*s1) {
		if (!memcmp((char*)s1, s2, len))
			return (char*)s1;
		
		s1++;
	}
	
	return NULL;
}

size_t
strspn(char const *s1, char const *s2) {
	const char *p;
	const char *a;
	size_t count = 0;

	for(p = s1; *p != '\0'; ++p) {
		for(a = s2; *a != '\0'; ++a) {
			if(*p == *a)
				break;
		}
		if(*a == '\0')
			return count;
		++count;
	}

	return count;
}

char*
strpbrk(char const *s1, char const *s2) {
	const char *sc1;
	const char *sc2;
	for(sc1 = s1; *sc1 != '\0'; sc1++) {
		for(sc2 = s2; *sc2 != '\0'; sc2++) {
			if(*sc1 == *sc2)
				return (char *)sc1;
		}
	}
	return NULL;
}

static char *___strtok = NULL;

char*
strtok(char *s1, char const *s2) {
	char *sbegin, *send;
	sbegin  = s1 ? s1 : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	
	sbegin += strspn(sbegin, s2);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return NULL;
	}
	
	send = strpbrk( sbegin, s2);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return sbegin;
}