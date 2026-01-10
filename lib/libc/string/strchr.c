
#include "sys\string.h"
#include "sys\stdint.h"

char* strchr(const char* str, int c) {	
	while (*str != (char) c) {
		if (*str == '\0') {
			return (char*)NULL;
		}
	}
	
	return (char*) str;
}
