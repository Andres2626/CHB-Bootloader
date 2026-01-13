
/*
 * fat.h -- FAT filesystem structure
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _FAT_H_
#define _FAT_H_ 1

#include "types.h"
#include "const.h"

#define FAT_MAGIC 0xAA55

/* 
 * ***_FS_IDENT: This is the number which CHB detects to know that it is working 
 * with a specific FS 
 */
#define EXFAT_FS_IDENT 0 /* not implemented */
#define FAT12_FS_IDENT 0x1
#define FAT16_FS_IDENT 0x2
#define FAT32_FS_IDENT 0x3
#define FAT_FS_LABEL "FAT"

/* FAT12 common definitions */
#define FAT12_BPB_SIZE          (sizeof(struct fat_bootsec))
#define FAT12_EOF               0xFF8

/* FAT16 common definitions */
#define FAT16_EOF 0xFFF8

/* FAT32 common definitions */
#define FAT32_EOF 0x0FFFFFF8
#define FAT32_RESERVED_MBR_SIZE 2048

/* FAT common definitions */
#define FAT_SECSIZE              512
#define FAT_MAX_PATHSIZE         256

/* FAT file flags */
#define FAT_RONLY                0x1
#define FAT_HIDDEN               0x2
#define FAT_SYSTEM               0x4
#define FAT_VID                  0x8
#define FAT_DIR                  0x10
#define FAT_ARCHIVE              0x20
#define FAT_LFN                  (FAT_READONLY | FAT_HIDDEN | FAT_SYSTEM | FAT_VOLUMEID)

struct device;

struct fat_boot_sector {
   u8t jmp[3];
   u8t oem[8];
   u16t sector_size;
   u8t cluster_size;
   u16t reserved_sectors;
   u8t FAT_count;
   u16t dir_entries; /* for FAT12/16 */
   u16t drive_size;
   u8t desc_type;
   u16t FAT_size;
   u16t track_size;
   u16t drive_heads;
   u32t hidden_sectors;
   u32t large_sectors;
   
   u32t fat32_size;
   u16t ext_flags;
   u16t version;
   u32t root_cluster;
   u16t FSInfo;
   u16t backup_boot;
   u8t  reserved2[12];
   
   /* EBR */
   u8t drive_number;
   u8t reserved;
   u8t signature;
   u32t volumeid;
   u8t label[11];
   u8t id[8];
} __attribute__(( packed ));

struct fat_entry {
   u8t name[11];
   u8t attributes;
   u8t reserved;
   u8t created_time_tenths;
   u16t created_time;
   u16t created_date;
   u16t accessed_date;
   u16t cluster_hi;
   u16t modified_time;
   u16t modified_date;
   u16t cluster_lo;
   u32t size;
} __attribute__(( packed ));


#ifndef CHB_UTIL

#include "fs.h"

PUBLIC int fat_mount(struct device *disk);
PUBLIC void fat_unmount();
PUBLIC int fat_open(_CONST char *path);
PUBLIC i32t fat_read(void *buff, u32t len);
PUBLIC int fat_close();

#endif

#endif /* !_FAT_H_ */