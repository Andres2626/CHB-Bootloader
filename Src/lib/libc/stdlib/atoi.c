
#include <CHB/stdlib.h>

int 
atoi(char* ptr) {
	int number = 0;
    int sign = 1;
    int i = 0;
 
	/* check if number is negative */
    if (ptr[0] == '-') {
        sign = -1;
        i++;
    }

    for (; ptr[i] != '\0'; i++)
        number = number * 10 + ptr[i] - '0';
 
    return sign * number;
}