
/* Define the start of BIOS parameter block */
#define BPB_START 0x04

/* Define the end of BIOS parameter block */
#define BPB_END 0x64

/* Define the localization of Bootloader */
#define BOOT_STACK_LOC 0x7c00

/* Define the start of MBR */
#define MBR_START 0x1b4

/* Define the end of MBR*/
#define MBR_END 0x1fe

/* Define signature of bootloader */
#define BOOT_SIGNATUTE 0xaa55

/* Macro for print messages */
#define MSG(x) 	mov $x, %si; call print

/* Macro for print error*/
#define ERROR(x) MSG(x); MSG(general_error); MSG(newline)
