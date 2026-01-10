
#include "sys\string.h"
#include "sys\stdint.h"

char* strtok(char* s1, char s2) {	
	char* buffer = NULL;
	
	if (s1 != NULL) {
		buffer = s1;
	}
	
	if (buffer == NULL) {
		return NULL;
	}
	
	char* ret;
	
	int index = 0;
	while (*buffer++ != '\0') {
		if (buffer[index] != s2) {
			*ret = *buffer;
		}
		else {
			*ret = '\0';
			*buffer = *buffer + index + 1;
			return ret;
		}
		
		ret++;
		index++;
	}
	
	*ret = '\0';
	ret++;
	buffer = NULL;
	
	return ret;
}
