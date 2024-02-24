
#include <CHB/stdio.h>

void
puts(const char *s) {
	while (*s) {
		putchar(*s);
		s++;
	}
}