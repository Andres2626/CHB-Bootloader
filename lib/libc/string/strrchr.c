
#include "sys\string.h"
#include "sys\stdint.h"

char* strrchr(const char* str, int c) {	
	char* buffer;
	while (*str != '\0') {
		if (*str == (char)c) {
			*buffer = *str;
		}
		
		buffer++;
		str++;
	}
	
	*buffer = '\0';
	
	return buffer;
}
