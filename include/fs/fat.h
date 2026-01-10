#ifndef _FAT_H_
#define _FAT_H_ 1

#ifndef USE_HOST
#include "sys/stdint.h" /* types */
#include "boot/stage1.h" /* BOOT_MAGIC */
#include "fsys.h" /* FILE */
#include "shared/disk.h" /* disk_geom_info_t */
#else
#include <stdint.h>
#endif

#define FAT1X_BOOT_SIZE 448
#define FAT32_BOOT_SIZE 420
#define FAT_ENTRY_

#ifdef _MSC_VER
#define __attribute__(x)
#endif

typedef struct fat_bootsec {
    uint8_t jmp_instruction[3]; /* boot jump code*/
    char oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t root_entry;
    uint16_t total_sectors;
    uint8_t media;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track; /* obtained by BIOS */
    uint16_t heads;             /* obtained by BIOS */
    uint32_t hidden_sectors;
    uint32_t total_fat32_sectors;
    union {
        struct {
            uint8_t drive_number; /* 0x0 - 0x7F floppy - >0x80 HDD */
            uint8_t reserved;
            uint8_t signature;
            uint8_t volume_id[4];
            uint8_t volume_name[11];
            uint8_t fat_name[8];
            uint8_t buffer[FAT1X_BOOT_SIZE];
        } __attribute__ ((packed)) _fat1x;
        struct {
            uint32_t sectors_per_fat;
            uint16_t flags;
            uint16_t fat_version_number;
            uint32_t root_directory_cluster;
            uint16_t fs_info_sector;
            uint16_t backup;
            uint8_t _reserved[12];
            uint8_t drive_number;
            uint8_t reserved;
            uint8_t signature;
            uint32_t volume_id;
            uint8_t volume_name[11];
            uint8_t fat_name[8];
            uint8_t buffer[FAT32_BOOT_SIZE];
        } __attribute__ ((packed)) _fat32;
    } __attribute__ ((packed)) fstype;
    uint8_t boot_signature;
} __attribute__((packed)) fat_bootsec_t;
#define fat32   fstype._fat32
#define fat1x   fstype._fat1x

typedef struct fat_dir_entry {
	char name[11]; /* bit 0-8 name, bit 9-11 extension */
	uint8_t flags; /* view fat_dir_flags for more info */
	uint8_t reserved; /* windows NT */
	uint8_t created_time_tenths;
	uint16_t created_time;
	uint16_t created_date;
	uint16_t last_accsesed;
	uint16_t first_cluster_high;
	uint16_t modified_time;
	uint16_t modified_date;
	uint16_t first_cluster_low;
	uint32_t file_size; /* in bytes */
} __attribute__((packed)) fat_dir_entry_t;

typedef enum fat_dir_flags {
	READ_ONLY = 0x1,
	HIDDEN = 0x2,
	SYSTEM = 0x4,
	VOLUME_ID = 0x8,
	DIRECTORY = 0x10,
	FIL = 0x20,
	LONG_NAME = READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID
} fat_dir_flags_t;

#ifndef USE_HOST
void fat_init(disk_geom_info_t dsk);
void fat_mount();
void fat_unmount();
FILE fat_dir(const char* dir);
void fat_read(FILE* file, char* buff, int len);
FILE fat_open(const char* file);
void fat_close(FILE* file);
#endif

#endif