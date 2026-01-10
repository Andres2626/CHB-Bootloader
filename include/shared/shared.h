
#ifndef SHARED_HEADER
#define SHARED_HEADER 1

/* protected mode -- CR0 */
#define CR0_ENABLE 		1
#define CR0_DISABLE 	0xfffffffe

/* 16-bit and 32-bit code/data segments */
#define PROT_16_CODE_SEG 	0x18
#define PROT_32_CODE_SEG 	0x8
#define PROT_32_DATA_SEG 	0x10
#define REAL_16_CODE_SEG 	0

/* VGA 80X25 defs */
#define VGA_HEIGHT 	25
#define VGA_WIDTH 	80
#define VGA_MEMORY 	(uint8_t*)0xb8000

/* VGA16 colors*/
#define FORE_BLACK 				0x00
#define FORE_BLUE 				0x01
#define FORE_GREEN 				0x02
#define FORE_CYAN 				0x03
#define FORE_RED 				0x04
#define FORE_MAGENTA 			0x05
#define FORE_BROWN 				0x06
#define FORE_LIGHTGRAY 			0x07
#define FORE_DARKGRAY 			0x08
#define FORE_LIGHTBLUE 			0x09
#define FORE_LIGHTGREEN 		0x0A
#define FORE_LIGHTCYAN 			0x0B
#define FORE_LIGHTRED 			0x0C
#define FORE_LIGHTMAGENTA 		0x0D
#define FORE_YELLOW 			0x0E
#define FORE_WHITE 				0x0F
#define BACK_BLACK 				0x00
#define BACK_BLUE 				0x10
#define BACK_GREEN 				0x20
#define BACK_CYAN 				0x30
#define BACK_RED 				0x40
#define BACK_MAGENTA 			0x50
#define BACK_BROWN 				0x60
#define BACK_LIGHTGRAY 			0x70
#define BACK_BLINKINGBLACK 		0x80
#define BACK_BLINKINGBLUE 		0x90
#define BACK_BLINKINGGREEN 		0xA0
#define BACK_BLINKINGCYAN 		0xB0
#define BACK_BLINKINGRED 		0xC0
#define BACK_BLINKINGMAGENTA 	0xD0
#define BACK_BLINKINGYELLOW 	0xE0
#define BACK_BLINKINGWHITE 		0xF0

/* memory type DEFS */
#define MEMORY_USABLE 		0x1
#define MEMORY_RESERVED 	0x2
#define MEMORY_ACPI_REC 	0x3
#define MEMORY_ACPI_NVS 	0x4
#define MEMORY_BAD 			0x5

#define FILESYSTEM_DRV_ADDRESS (void*)0x20000

#ifndef ASM_FILE

#include "sys/stdint.h"

void load_new_gdt(); // gdt.S

/* memory structures */
enum memory_type {
	USABLE = 1,
	RESERVED,
	ACPI_RECLAIMABLE,
	ACPI_NVS,
	BAD_MEMORY,
};

typedef struct memory_block {
	uint64_t begin;
	uint64_t length;
	uint32_t type;
	uint32_t ACPI;
} memory_block_t;

typedef struct memory_info {
	int lower_size;
	int upper_size;
	memory_block_t* blocks;
	int block_count;
} memory_info_t;

/* 
 * disk types
 * define the drive number provided by BIOS
 */
#define DRV_FLOPPY 0x0
#define DRV_HDD 0x80

typedef struct disk_geom_info {
	uint8_t drive_number;
	uint16_t type;
	uint16_t cylinders;
	uint16_t heads;
	uint16_t sectors;
} disk_geom_info_t;

/* asm.S -- DEFS */

/* video mode*/
void set_video_mode(int mode);

/* Memory functions */
int get_lower_memory		(void);
int get_upper_memory		(void);
int get_memory_block		(memory_block_t* block, uint32_t* continuation);

/* DISK Controller functions*/
int disk_parameters			(uint8_t drive_number, uint8_t* type, uint16_t* cylinder, uint16_t* sector, uint16_t* head);
int disk_read				(uint8_t drive_number, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t sector_count, void* buffer);
int disk_reset				(int drive_number);
uint8_t get_drive_number(void);

/* I/O Assembly functions */
void outb	(uint16_t port, uint8_t value); /* output */
uint8_t inb	(uint16_t port); /* input */

/* kill the program */
void stop();

#endif

#endif