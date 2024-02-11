OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(entry)

/* set the location of bootloader */
loc = 0x8000;

SECTIONS
{
	/* set localization of our stage1 in memory */
	. = loc;
	
	.text : { __text_start = .;       *(.text)    }
    .data : { __data_start = .;       *(.data)    }
    .rodata : { __rodata_start = .;     *(.rodata)  }
    .bss : { __bss_start = .;        *(.bss)     }
    
    _end = .;
	
	/* ignore unessesary sections */
	/DISCARD/ : { *(.comment .note .eh_frame .dtors .stab .stabstr .debug*) }
}

