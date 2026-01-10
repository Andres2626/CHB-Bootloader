
#include "sys\string.h"
#include "sys\stdint.h"

const char* strpbrk(const char* s1, const char* s2) {	
	if((s1 == NULL) || (s2 == NULL)) {
		return NULL;
	}
	
	while (*s1) {
		if (strchr(s2, *s1)) {
			return s1;
		}
		else {
			s1++;
		}
	}
}
