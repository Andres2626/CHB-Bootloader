
#ifndef _STAGE1_H_
#define _STAGE1_H_ 1

/* the first part size */
#define BOOT_PART_SIZE 510

/* magic boot number */
#define BOOT_MAGIC 0xaa55

/* BIOS parameter block start */
#define BOOT_BPB_START 0x4

/* BIOS parameter block end */
#define BOOT_BPB_END 0x5a

/* define the total bootblock size */
#define BOOT_TOTAL_SIZE 1024

/* define the localization of stack */
#define BOOT_LOC 0x7c00

/* define the localization of disk buffer */
#define BOOT_DISK_BUFFER 0x2000

/* define the MBR start */
#define MBR_START 0x1b8

/* define MBR partiion start */
#define MBR_PARTITION_START 0x1be

#endif /* !_STAGE1_H_ */