
#include <CHB/string.h>

size_t 
strlen(const char *s) {
	size_t index;
	while (*s != '\0') {
		*s++;
		index++;
	}
	
	return index;
}