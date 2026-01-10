
#include "sys\stdlib.h"
#include "sys\stdio.h"

void simple_exit(int code) {
	/*TODO: Improve this*/
	printf("exit... code %i\n", code);
	for (;;); /* never returns */
}
