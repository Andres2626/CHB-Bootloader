
/*
* fat.c -- simple FAT FS controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <fs/fat.h>

#include <sys/mm.h>

#include <sys/disk.h>

#include "lib/libc/ctype.h"
#include "lib/libc/errno.h"
#include "lib/libc/minmax.h"
#include "lib/libc/stdio.h"
#include "lib/libc/string.h"

/* FAT filesystem misc structures */
struct fat_boot_sector {
   u8t jmp[3];
   u8t oem[8];
   u16t sector_size;
   u8t cluster_size;
   u16t reserved_sectors;
   u8t FAT_count;
   u16t dir_entries;
   u16t drive_size;
   u8t desc_type;
   u16t FAT_size;
   u16t track_size;
   u16t drive_heads;
   u32t hidden_sectors;
   u32t large_sectors;

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

struct fat_internal_file {
    struct file f;
    bool dir;
    u32t lba;
    u32t first_cluster;
    u32t current_cluster;
    u32t current_sector;
    u8t buffer[FAT_SECSIZE];
};

struct fat_fs {    
    union {
        struct fat_boot_sector info;
        u8t buffer[FAT_SECSIZE];
    } b;
    
    struct fat_internal_file root;
    
    struct {
        u32t lba;
        u32t size;
    } data;
};

PRIVATE struct device *fs_dev;
PRIVATE struct fat_fs *fs; /* FS info */
PRIVATE u8t *fs_tab; /* The file allocation table */

PRIVATE int read_bsec()
{
    if (!fs_dev)
        return 0; /* fail */
    
    return devread(fs_dev, 0, 1, fs->b.buffer);
}

PRIVATE int read_fat()
{
    if (!fs_dev)
        return 0; /* fail */
    
    /* set location of 'FAT' in memory */
    fs_tab = (u8t*)(FS_DRIVER_DATA + sizeof(struct fat_fs));
    return devread(fs_dev, fs->b.info.reserved_sectors, fs->b.info.FAT_size, fs_tab);
}

PRIVATE int read_root(u32t sector)
{
    /* get info */
    u32t size = sizeof(struct fat_entry) * fs->b.info.dir_entries;
    u32t root_sec = (size + fs->b.info.sector_size - 1) / fs->b.info.sector_size;
    u32t begin = (fs->b.info.reserved_sectors + (fs->b.info.FAT_size * fs->b.info.FAT_count));
    
    /* check if sector is not out of root directory */
    if (sector >= root_sec)
        goto fail;
    
    if (!fs_dev)
        goto fail;
    
    /* get root info */
    fs->root.f.dir = true;
    fs->root.f.size = size;
    fs->root.f.pos = 0;
    fs->root.lba = begin + sector;
    fs->root.first_cluster = fs->root.lba;
    fs->root.current_cluster = fs->root.lba;
    fs->root.current_sector = sector;
    
    /* read root */
    return devread(fs_dev, fs->root.lba, 1, fs->root.buffer);
fail:
    return 0;
}

PRIVATE uint32_t fat_to_lba(uint32_t cluster) 
{
   return fs->data.lba + (cluster - 2) * fs->b.info.cluster_size;
}

PRIVATE u16t fat_next_cluster(u16t cluster)
{
    u32t offset = (cluster * 3) / 2;
    u16t val = *(u16t*)(fs_tab + offset);

    if (cluster & 1)
        val >>= 4;
    else
        val &= 0xFFF;

    return val;
}

PRIVATE char *fat_path_to_dos(_CONST char *path)
{
    PRIVATE char fatname[12];
    _CONST char *dot = strchr(path, '.');

    memset(fatname, ' ', 11);
    fatname[11] = '\0';

    size_t len = dot ? (size_t)(dot - path) : strlen(path);
    if (len > 8)
        len = 8;

    for (size_t i = 0; i < len; i++) {
        fatname[i] = toupper((unsigned char)path[i]);
    }

    if (dot) {
        for (size_t i = 0; i < 3 && dot[i + 1]; i++) {
            fatname[8 + i] = toupper((unsigned char)dot[i + 1]);
        }
    }

    return fatname;
}

struct fs *fat_detect_fs(struct device *disk)
{
    PRIVATE struct fs f;
    fs_dev = disk;
    fs = (struct fat_fs*)FS_DRIVER_DATA;
    
    if (!fs_dev || fs_dev->state != DRIVE_STATE_1) {
        printf("[FAT] Disk is not initialized %i\n");
        errno = EDEVINT;
        goto fail;
    }
    
    if (!read_bsec()) {
        printf("[FAT] Error reading bootsector\n");
        goto fail;
    }
    
    /* check valid FAT format */
    u16t sig = (fs->b.buffer[511] << 8) | fs->b.buffer[510];
    if (sig == FAT_MAGIC)
        goto success;
    
    errno = EIFS;
fail:
    return 0;
success:
    /* push FS descriptor */
    f.type = FAT12_TYPE;
    f.fs_data = (void*)fs;
    f.fs_dev = fs_dev;
    return &f;
}

int fat_mount(struct device *disk)
{   
    u32t fat_size = 0;
    fs_dev = disk;
    
    if (!fs_dev || fs_dev->state != DRIVE_STATE_1) {
        printf("[FAT] Disk is not initialized %i\n");
        errno = EDEVINT;
        goto fail;
    }
    
    /* FAT driver only accepts multiples of 512 in sector size */
    if ((fs->b.info.sector_size % 512) != 0) {
        printf("[FAT] Invalid sector size %i\n", fs->b.info.sector_size);
        errno = EIFS;
        goto fail;
    }
        
    fat_size = fs->b.info.FAT_size * fs->b.info.sector_size;
    if ((sizeof(struct fat_fs) + fat_size) > FS_DRIVER_DATA_SIZE) {
        errno = ENOMEM;
        goto fail;
    }
    
    if (!read_fat()) {
        printf("[FAT] Error reading table\n");
        goto fail;
    }
    
    if (!read_root(0)) {
        printf("[FAT] Error reading root\n");
        goto fail;
    }
    
    /* get data info */
    fs->data.size = (fs->root.f.size + fs->b.info.sector_size - 1);
    fs->data.size /= fs->b.info.sector_size;
    fs->data.lba = fs->data.size + fs->root.f.size;
    return 1;
fail:
    return 0;
}

PUBLIC void fat_unmount()
{
    if (!fs)
        goto out;

    if (fs_tab)
        memset(fs_tab, 0, fs_tab_size);
    
    fs->root.first_cluster = 0;
    fs->root.lba = 0;
    memset(fs->root.buffer, 0, sizeof(fs->root.buffer));
}

struct file *fat_open(_CONST char *path)
{
    if (!path || !fs || !fs_dev) 
        goto fail; /* filesystem not mounted */
    
    PRIVATE struct file f;
    bool found = false; /* is file found? */
    size_t plen = strlen(path);
    u16t entries_per_sector = fs->b.info.sector_size / sizeof(struct fat_entry);
    char tmp[FAT_MAX_PATHSIZE];
    struct fat_internal_file dir = fs->root;
    struct fat_entry *entry;
    
    /* copy path to tmp buffer */
    memcpy(tmp, path, plen + 1);
    
    if (plen >= sizeof(tmp)) 
        goto fail; /* invalid path size */
    
    /* tokenize path */
    char *tok = strtok(tmp, "/");
    if (!tok) 
        goto fail; /* invalid token */
    
    while (tok) {
        char *fatname = fat_path_to_dos(tok);
        found = false;

        /* check if current dir is root dir TODO: this not work in FAT32 */
        if (dir.first_cluster == fs->root.first_cluster) {
            u32t size = sizeof(struct fat_entry) * fs->b.info.dir_entries;
            u32t total_sectors = (size + fs->b.info.sector_size - 1) / fs->b.info.sector_size;
            
            for (u32t s = 0; s < total_sectors; s++) {
                if (!read_root(s))
                    goto fail;

                struct fat_entry *entries = (struct fat_entry*)fs->root.buffer;

                for (u32t i = 0; i < entries_per_sector; i++) {
                    if (entries[i].name[0] == 0x00)
                        goto end_subdir_scan; 
                    else if (entries[i].name[0] == 0xE5)
                        continue; /* deleted */

                    /* check if entry name is equal to fat name */
                    if (!memcmp(entries[i].name, fatname, 11)) {
                        entry = &entries[i];
                        found = true;
                        break;
                    }
                }
                
                if (found)
                    break;
            }     
        }
        else {
            u16t cluster = dir.first_cluster;
            while (cluster < FAT_EOF12) {
                u32t lba = fat_to_lba(cluster);
                
                for (u32t s = 0; s < fs->b.info.cluster_size; s++) {
                    if (!devread(fs_dev, lba + s, 1, dir.buffer))
                        goto fail;

                    struct fat_entry *entries = (struct fat_entry*)dir.buffer;
                    for (u32t i = 0; i < entries_per_sector; i++) {
                        
                        if (entries[i].name[0] == 0x00)
                            goto end_subdir_scan;
                        else if(entries[i].name[0] == 0xE5)
                            continue; /* deleted entry */
                        
                        if (!memcmp(entries[i].name, fatname, 11)) {
                            entry = &entries[i];
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
                if (found)
                    break;
                
                /* recalculate cluster */
                cluster = fat_next_cluster(cluster);
            }
        }
        
end_subdir_scan:
        if (!found)
            goto fail;

        tok = strtok(NULL, "/");
        if (tok) {
            
            /* check if entry is 'file' */
            if (!(entry->attributes & FAT_DIR))
                goto fail;
            
            /* reload internal file descriptor */
            dir.first_cluster = entry->cluster_lo;
            dir.lba = fat_to_lba(entry->cluster_lo);
            dir.f.dir = true;
        }
        else
        {
            /* push file descriptor to 'VFS' */
            f.dir = false;
            f.pos = 0;
            f.size = entry->size;
            return &f;
        }
    }

fail:
    return NULL;
}

