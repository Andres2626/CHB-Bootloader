
#include <CHB/string.h>

void* 
memcpy(void *s1, const void *s2, size_t n) {
	/*set the arrays (char[])*/
	char* cdest = (char*)s1;
	char* csrc = (char*)s2;

	int index = 0;
	while (index < n) {
		*cdest = *csrc;
		cdest++;
		csrc++;
		index++;
	}
	
	return cdest;
}
