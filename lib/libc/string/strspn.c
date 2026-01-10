
#include "sys\string.h"
#include "sys\stdint.h"

size_t strspn(const char* s1, const char* s2) {	
	size_t count = 0;
	if ((s1 == NULL) || (s2 == NULL)) {
		return count;
	}
	
	while (*s1 && strchr(s2, *s1++)) {
		count++;
	}
	
	return count;
}
