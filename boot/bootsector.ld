OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(_start)

/* set the location of bootloader */
loc = 0x7c00;

SECTIONS
{
	/* set localization of our bootsector in memory */
	. = loc;

	/* save the main sections .text .rodata .data and .bss */
	.boot : { *(.text .rodata .data .bss) }
	
	/* no save unessesary sections */
	/DISCARD/ : { *(.comment .note .eh_frame .dtors .stab .stabstr .debug*) }
}

