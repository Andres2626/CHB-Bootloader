
#include "sys\string.h"
#include "sys\stdint.h"

void* memchr(const void* buff, int ch, size_t n) {
	const unsigned char *b= buff;
	unsigned char x= (ch&0xff);

	for(size_t i= 0; i< n; i++) {
		if(b[i]== x) {
			return (void*)(b+i);
		}
	}

	return NULL;
}
