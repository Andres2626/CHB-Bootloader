
#include "sys\string.h"
#include "sys\stdint.h"

void* memmove(void* dest, const void* src, size_t n) {
	/*set the arrays (char[])*/
	char* cdest = (char*)dest;
	char* csrc = (char*)src;

	int index = 0;
	while (index < n) {
		*cdest = *csrc;
		
		cdest++;
		csrc++;
		index++;
	}
	
	return cdest;
}
