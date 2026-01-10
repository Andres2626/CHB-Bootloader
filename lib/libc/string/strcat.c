
#include "sys\string.h"
#include "sys\stdint.h"

char* strcat(char* dest, const char* src) {
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	
	*dest = '\0';
	
	return dest;
}
