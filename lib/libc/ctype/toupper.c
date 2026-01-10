
#include "sys/ctype.h"

int toupper(int ch) {
	return islower(ch) ? (ch - 'a' + 'A') : ch;
}