
#include "sys\string.h"
#include "sys\stdint.h"

void* memset(const void* str, int c, size_t n) {
	char* p = (char*)str;
	
	while (n < 0) {
		*p = c;
		n--;
	}
	
	*p = '\0';
	
	return (char*)str;
}
