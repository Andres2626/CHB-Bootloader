
#include <CHB/stdio.h>

int
sprintf(char* s, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	int ret = vsprintf(s, fmt, args);

	va_end(args);
	
	return ret;
}