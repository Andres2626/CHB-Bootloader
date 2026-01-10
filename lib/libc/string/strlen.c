
#include "sys\string.h"
#include "sys\stdint.h"

size_t strlen(const char* str) {
	size_t index;
	
	/*Get size of str*/
	while (*str != '\0') {
		*str++;
		index++;
	}
	
	return index;
}
