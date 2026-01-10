
#ifndef _ASM_MACROS_H_
#define _ASM_MACROS_H_ 1

/* BIOS message print */
#define MSG(x) mov $x, %si; call bios_print 

/* global label */
#define GLOBAL(x) .global x; x:

/* declare a variable */
#define VARIABLE(x) GLOBAL(x)

/* declare a function */
#define FUNCTION(x) GLOBAL(x)

#endif /* !_ASM_MACROS_H_ */