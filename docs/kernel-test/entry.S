
/* entry.S -- entry of the kernel */

.code32

.text

.global _start
_start:
	/* call C function */
	call main

/* Infinite loop. */
stop:
	jmp stop;
	