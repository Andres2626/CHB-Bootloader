
/*
 * fat.h -- FAT filesystem structure
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _FAT_H_
#define _FAT_H_ 1

#include <CHB/types.h>

/* This is the size of BIOS parameter block (FAT12) */
#define BPB_SIZE12            sizeof(struct fat_bootsec)

/* BIOS disk default sector size. */
#define SEC_SIZE              512

/* Max filename size */
#define MAX_PATH_SIZE         256

/* Filesystem driver maximum opened files */
#define MAX_FILE_HANDLES      10

/* Used for root directory */
#define ROOT_DIRECTORY_HANDLE -1

/* This is the value of the END of cluster in FAT12 */
#define FAT12_EOF             0xFF8

/* BIOS disk structure -- defined in disk.h */
struct device;

struct FAT_file {
   int handle;
   bool directory;
   uint32_t pos;
   uint32_t size;
   bool opened;
   uint32_t first_cluster;
   uint32_t current_cluster;
   uint32_t current_sector;
   uint8_t buffer[SEC_SIZE];
};

/* Attribute of file */
enum FAT_flags {
   FAT_READONLY = 0x01,
   FAT_HIDDEN = 0x02,
   FAT_SYSTEM = 0x04,
   FAT_VOLUMEID = 0x08,
   FAT_DIRECTORY = 0x10,
   FAT_ARCHIVE = 0x20,
   FAT_LFN = FAT_READONLY | FAT_HIDDEN | FAT_SYSTEM | FAT_VOLUMEID
};

int fat_init(struct device* dev);
uint32_t fat_read(struct device* dev, struct FAT_file* file, uint32_t count, void* buff);
uint32_t fat_write(); /* TODO: Not implemented. */
struct FAT_file* fat_open(struct device* dev, const char* path);
void fat_close(struct FAT_file* file);

#endif /* !_FAT_H_ */