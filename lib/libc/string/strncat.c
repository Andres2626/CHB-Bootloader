
#include "sys\string.h"
#include "sys\stdint.h"

char* strncat(char* dest, const char* src, size_t n) {	
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
		
		n--;
		
		if (n < 0) {
			break;
		}
		
	}
	*dest = '\0';
	
	return dest;
}
