
/* kernel.c -- C part of the kernel*/

void main() {
	
	/* print "hi" */
	*((int*)0xb8000)=0x07690748;
	
	/* infinite loop */
	for(;;);
}