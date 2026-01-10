
#include "sys\string.h"
#include "sys\stdint.h"

char* strcpy(char *s1, const char* s2)
{
	char* tmp = s1;

	while((*s1++ = *s2++) != '\0');
	
	return tmp;
}
