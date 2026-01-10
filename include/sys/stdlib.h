
#ifndef _STDLIB_H_
#define _STDLIB_H_ 1

#define EXIT 		0
#define BAD_EXIT 	1

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

char* itoa(int value, char* buffer, int base); /* lib/stdlib/misc.c */
void simple_exit(int code); /* lib/stdlib/exit.c */

#endif /* !_STDLIB_H_ */