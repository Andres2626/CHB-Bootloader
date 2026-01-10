
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
	
	/* convert linear segment to memory segment */
	.macro linear_to_segment bp, es, esi, si
		mov \bp, \esi     
		shr $4, \esi
		mov \si, \es
		mov \bp, \esi
		and $0xf, \esi
	.endm
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