
#include "sys\string.h"
#include "sys\stdint.h"

char* strstr(char* str, char* substr)
{	
	char* buffer;
	
	buffer = str;
	
	while (buffer++)
	{
		if (strncmp(buffer, substr, strlen(substr)) == 0)
			return buffer;
	}
}
