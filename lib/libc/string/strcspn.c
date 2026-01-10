
#include "sys\string.h"
#include "sys\stdint.h"

size_t strcspn(const char* s1, const char* s2) {	
	size_t count;
	
	if ((s1 == NULL) || (s2 == NULL)) {
		return count;
	}
	
	while (*s1) {
		if (strchr(s2, *s1)) {
			return count;
		}
		else {
			s1++;
			count++;
		}
	}
	
	return count;
}
