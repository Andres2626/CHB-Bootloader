
#include "sys\string.h"
#include "sys\stdint.h"

char* strrev(char* str) {
	char* buffer;
	size_t len = strlen(str) + 1;
	
	while (*str) {
		*buffer = str[len--];
		
		str++;
		buffer++;
		
		if (len < 0) {
			break;
		}
	}
	
	*buffer = '\0';
	buffer++;
	
	return buffer;
}
