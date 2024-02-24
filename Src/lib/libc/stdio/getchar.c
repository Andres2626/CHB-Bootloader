
#include <CHB/stdio.h>

int 
getchar(void) {
	int key = keyboard_get_key_status() & 0xFF;
	return key;
}