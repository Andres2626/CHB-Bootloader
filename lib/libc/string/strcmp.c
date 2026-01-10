
#include "sys\string.h"
#include "sys\stdint.h"

int strcmp(const char* s1, const char* s2) {	
	int compare = 0;
    while( ( *s1 != '\0' && *s2 != '\0' ) && *s1 == *s2 ) {
        s1++;
        s2++;
    }
	
    compare = (*s1 ==*s2)?0:(*s1 >*s2)?1:-1;
    return compare;
}
