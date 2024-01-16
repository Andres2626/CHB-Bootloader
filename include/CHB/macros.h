
#ifndef _ASM_MACROS_H_
#define _ASM_MACROS_H_ 1

#if defined (STAGE1_FILE) || defined (LOADER_FILE)
	/* 
	 * BIOS message print
	 * NEVER use this macro on protected mode!
	 */
	#define MSG(x) mov $x, %si; call _print 

#endif /* STAGE1_FILE */

#if defined (ASM_FILE)
	/* global label */
	#define GLOBAL(x) .global x; x:
	/* declare a variable */
	#define VARIABLE(x) GLOBAL(x)
	/* declare a function */
	#define FUNCTION(x) GLOBAL(x)
	/* declare section */
	#define SECTION(x) .##x
#else
	/* global function */
	#define GLOBAL(x) x
	/* declare a variable */
	#define VARIABLE(x) GLOBAL(x)
	/* declare a function */
	#define FUNCTION(x) GLOBAL(x)
	/* declare section */
	#define SECTION(x) x
#endif /* ASM_FILE */

#endif /* !_ASM_MACROS_H_ */