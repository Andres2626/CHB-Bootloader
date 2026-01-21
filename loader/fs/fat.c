
/*
* fat.c -- simple FAT FS controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

/* 
 * At the moment the driver found FAT32 from first partition with type 0x0B or 0x0C 
 * TODO: Improve this!
 */

#include <fs/fat.h>

#include <sys/mm.h>

#include <sys/disk.h>

#include "lib/libc/ctype.h"
#include "lib/libc/errno.h"
#include "lib/libc/minmax.h"
#include "lib/libc/stdio.h"
#include "lib/libc/string.h"
#include "lib/libc/stdlib.h"

struct fat_inode {
    bool opened;
    bool dir;
    bool root;
    u32t lba;
    u32t pos;
    u32t first_cluster;
    u32t current_cluster;
    u32t current_sector;
    struct fat_entry entry;
    u8t buffer[FAT_SECSIZE];
};

struct fat_fs { 
    /* Type of FAT filesystem */
    u8t type;
    u32t sec_count;
    u32t data_clusters;
    u32t data_size;
    
    union {
        struct fat_boot_sector info;
        u8t buffer[FAT_SECSIZE];
    } b;
    
    struct fat_inode root;

    struct {
        u32t lba;
        u32t size;
    } data;
    
    struct fat_inode file;
};

/* durrent disk */
PRIVATE struct device *fs_dev;

/* main FAT info */
PRIVATE struct fat_fs *fs;

/* file allocation table */
PRIVATE u8t *fs_tab;

/* DOS name buffer */
PRIVATE char fatname[12];

/* partition first sector LBA */
PRIVATE u32t part_index = 0;

/* Read partition */
#define PART_READ(disk, sector, count, buff) devread((disk), (sector + part_index), (count), (buff))

PRIVATE int read_bsec()
{
    if (!fs_dev)
        return SIGN(EFAULT); /* fail */
    
    return PART_READ(fs_dev, 0, 1, fs->b.buffer);
}

PRIVATE u32t fat_to_lba(u32t cluster) 
{
   return fs->data.lba + (cluster - 2) * fs->b.info.cluster_size;
}

PRIVATE u32t fat_next_cluster(u16t cluster)
{
    u32t ret = 0xFFFFFFFF;
    u32t fat_sector;
    u32t offset;

    /* check if disk is initialized */
    if (!fs_dev) {
        ret = 0xFFFFFFFF;
        goto out;
    }
    
    switch (fs->type) {
    case FAT12_FS_IDENT:
        fat_sector = fs->b.info.reserved_sectors + (cluster * 3) / 2 / 512;
        offset = (cluster * 3) / 2 % 512;
        break;
    case FAT16_FS_IDENT:
        fat_sector = fs->b.info.reserved_sectors + (cluster * 2) / 512;
        offset = (cluster * 2) % 512;
        break;
    case FAT32_FS_IDENT:
        fat_sector = fs->b.info.reserved_sectors + (cluster * 4) / 512;
        offset = (cluster * 4) % 512;
    }
    
    if (PART_READ(fs_dev, fat_sector, 1, fs_tab) < 0) {
        ret = 0xFFFFFFFF;
        goto out;
    }

    switch (fs->type) {
    case FAT12_FS_IDENT:
        ret = *(u16t*)(fs_tab + offset);
        ret = (cluster & 0x1) ? (ret >> 4) : (ret & 0xFFF);
        break;
    case FAT16_FS_IDENT:
        ret = *(u16t*)(fs_tab + offset);
        break;
    case FAT32_FS_IDENT:
        ret = *(u32t*)(fs_tab + offset) & 0xFFFFFFF;
        break;
    default:
        ret = 0xFFFFFFFF;
        break;
    }
    
out:
    return ret;
}

PRIVATE int read_root(u32t sector)
{ 
    if (!fs_dev)
        return SIGN(EFAULT);
    
    u32t lba = 0;
    u32t first_cluster = 0;
    u32t current_cluster = 0;
    u32t sector_in_cluster = 0;
    
    fs->root.root = true;
    fs->root.dir = true;
    fs->root.pos = 0;
    
    if (fs->type == FAT32_FS_IDENT) {
        first_cluster = fs->b.info.root_cluster;
        current_cluster = first_cluster;
        
        /* convert cluster to sector */
        u32t cluster_index = sector / fs->b.info.cluster_size;
        sector_in_cluster = sector % fs->b.info.cluster_size;
        
        for (u32t i = 0; i < cluster_index; i++) {
            current_cluster = fat_next_cluster(current_cluster);
            if (current_cluster >= FAT32_EOF)
                return SIGN(EINVAL); /* check if sector is not out-of-bounds */
        }
        
        /* get lba sector of cluster */
        lba = fat_to_lba(current_cluster) + sector_in_cluster;
    }
    else {
        /* get info */
        u32t size = sizeof(struct fat_entry) * fs->b.info.dir_entries;
        u32t root_sec = (size + fs->b.info.sector_size - 1) / fs->b.info.sector_size;
        u32t begin = (fs->b.info.reserved_sectors + (fs->b.info.FAT_size * fs->b.info.FAT_count));

        /* check if sector is not out of root directory */
        if (sector >= root_sec)
            return SIGN(EINVAL);
        
        /* get sector */
        lba = begin + sector;
    }
    
    fs->root.lba = lba;
    fs->root.first_cluster = first_cluster;
    fs->root.current_cluster = current_cluster;
    fs->root.current_sector = sector_in_cluster;
    
    /* read root */
    return PART_READ(fs_dev, fs->root.lba, 1, fs->root.buffer);
}

PRIVATE char *fat_path_to_dos(_CONST char *path)
{
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

int fat_mount(struct device *disk)
{   
    int ret = 0;
    fs_dev = disk;
    fs = (struct fat_fs*)FS_DRIVER_DATA;
    memset(fs, 0, sizeof(struct fat_fs));
    
    if (fs_dev->state != DRIVE_OK) {
        ret = SIGN(EIO);
        goto fail;
    }
    
    if (!disk->hdd)
        goto no_part;
    
    /* check first partition with 0xB or 0xC type TODO: Improve this! */
    for (int i = 0; i < 4; i++) {        
        struct mbr_partition_entry *entry = &disk->part.entry[i];
        
        /* check type of partition */
        if ((entry->type == 0x0B || entry->type == 0x0C) && 
            (entry->flags & 0x80) != 0) { 
            
            part_index = entry->lba;
            
            break; /* FAT32 partition found */
        }
    }
    
no_part:
    
    /* set localizacion of FAT */
    fs_tab = (u8t*)(FS_DRIVER_DATA + sizeof(struct fat_fs));
    
    /* probe FAT filesystem */
    if (read_bsec() < 0)
        goto fail;
    
    /* check valid FAT format */
    u16t sig = (fs->b.buffer[511] << 8) | fs->b.buffer[510];
    if (sig != FAT_MAGIC) {
        ret = SIGN(EIFS);
        goto fail;
    }
    
    /* FAT driver only accepts multiples of 512 in sector size */
    if ((fs->b.info.sector_size % 512) != 0) {
        ret = SIGN(EIFS);
        goto fail;
    }
    
    /* check if drive is FAT32 */
    u32t fatsize;
    bool is_fat32 = (fs->b.info.dir_entries == 0 && fs->b.info.FAT_size == 0 && fs->b.info.fat32_size != 0);
    if (is_fat32) {
        fs->type = FAT32_FS_IDENT;
        fatsize  = fs->b.info.fat32_size;
        fs->data.size = 0;
        
        /* check root is in safe cluster */
        if (fs->b.info.root_cluster < 2) {
            ret = SIGN(EIFS);
            goto fail;
        }
    }
    else {
        /* fot FAT1X */
        fatsize = fs->b.info.FAT_size;
        fs->data.size = (fs->b.info.dir_entries * 32 + fs->b.info.sector_size - 1) / fs->b.info.sector_size;
    }
    
    /* calculate data section */
    fs->data.lba = fs->b.info.reserved_sectors + (fs->b.info.FAT_count * fatsize) + fs->data.size;
    fs->sec_count = (fs->b.info.drive_size == 0) ? fs->b.info.large_sectors : fs->b.info.drive_size;
    
    if (fs->sec_count <= fs->data.lba) {
        ret = SIGN(EIFS);
        goto fail;
    }
    
    /* calculate data info */
    fs->data_size = fs->sec_count - fs->data.lba;
    fs->data_clusters = u64_div(fs->data_size, fs->b.info.cluster_size);

    /* detect FAT1X */
    if (!is_fat32) {
        if (fs->data_clusters < 4085)
            fs->type = FAT12_FS_IDENT;
        else if (fs->data_clusters < 65525)
            fs->type = FAT16_FS_IDENT;
        else {
            ret = SIGN(EIFS);
            goto fail;
        }
    }
    
    ret = read_root(0);
fail:
    return ret;
}

PUBLIC void fat_unmount()
{
    fs = NULL;
    fs_dev = NULL;
    fs_tab = NULL;
}

int fat_open(_CONST char *path)
{
    if (!path || !fs || !fs_dev) 
        return SIGN(EFAULT); /* filesystem not mounted */
    
    struct fat_inode cur = fs->root;
    struct fat_entry entry;
    bool found = false; /* is file found? */
    size_t plen = strlen(path);
    u16t entries_per_sector = fs->b.info.sector_size / sizeof(struct fat_entry);
    char tmp[FAT_MAX_PATHSIZE];
    
    /* copy path to tmp buffer */
    memcpy(tmp, path, plen + 1);
    
    if (plen >= sizeof(tmp)) 
        return SIGN(EINVAL); /* invalid path size */
    
    if (fs->file.opened == true)
        return SIGN(EMFILE); /* too many opened files */
    
    /* tokenize path */
    char *tok = strtok(tmp, "/");
    if (!tok) 
        return SIGN(EFAULT); /* invalid token */
    
    int ret = 0;
    while (tok) {
        char *fatname = fat_path_to_dos(tok);
        found = false;

        if (cur.root && fs->type != FAT32_FS_IDENT) {
            u32t size = sizeof(struct fat_entry) * fs->b.info.dir_entries;
            u32t total_sectors = (size + fs->b.info.sector_size - 1) / fs->b.info.sector_size;
            
            for (u32t s = 0; s < total_sectors; s++) {
                ret = read_root(s);
                if (ret < 0)
                    goto out;

                struct fat_entry *entries = (struct fat_entry*)fs->root.buffer;

                for (u32t i = 0; i < entries_per_sector; i++) {
                    if (entries[i].name[0] == 0x00)
                        goto end_subdir_scan; 
                    else if (entries[i].name[0] == 0xE5)
                        continue; /* deleted */

                    /* check if entry name is equal to fat name */
                    if (!memcmp(entries[i].name, fatname, 11)) {
                        entry = entries[i];
                        found = true;
                        goto end_subdir_scan;
                    }
                }
            }     
        }
        else {
            /* obtain end of chain */
            u32t eof = (fs->type == FAT32_FS_IDENT) ? FAT32_EOF : (fs->type == FAT16_FS_IDENT) ?
                       FAT16_EOF : FAT12_EOF;
                       
            u32t cluster = cur.first_cluster;
            
            while (cluster < eof) {
                u32t lba = fat_to_lba(cluster);
                
                for (u32t s = 0; s < fs->b.info.cluster_size; s++) {
                    ret = PART_READ(fs_dev, lba + s, 1, cur.buffer);
                    if (ret < 0)
                        goto out;

                    struct fat_entry *entries = (struct fat_entry*)cur.buffer;
                    for (u32t i = 0; i < entries_per_sector; i++) {
                        
                        if (entries[i].name[0] == 0x00)
                            goto end_subdir_scan;
                        else if(entries[i].name[0] == 0xE5)
                            continue; /* deleted entry */
                        
                        if (!memcmp(entries[i].name, fatname, 11)) {
                            entry = entries[i];
                            found = true;
                            goto end_subdir_scan;
                        }
                    }
                }
                
                /* recalculate cluster */
                cluster = fat_next_cluster(cluster);
            }
        }
        
end_subdir_scan:
        if (!found) {
            ret = SIGN(EFILE);
            goto out;
        }

        tok = strtok(NULL, "/");
        u32t cluster = (fs->type == FAT32_FS_IDENT) ? entry.cluster_lo | (entry.cluster_hi << 16) : entry.cluster_lo;
        
        if (tok) {
            
            /* check if entry is 'file' */
            if (!(entry.attributes & FAT_DIR)) {
                ret = -EIFS;
                goto out;
            }
            
            /* reload internal file descriptor */
            cur.first_cluster = cluster;
            cur.current_cluster = cluster;
            cur.lba = fat_to_lba(cluster);
            cur.dir = true;
            cur.root = false;
        }
        else
        {   
            /* push FAT info */
            fs->file.opened = true;
            fs->file.dir = false;
            fs->file.root = false;
            fs->file.first_cluster = cluster;
            fs->file.current_cluster = cluster;
            fs->file.current_sector = 0;
            fs->file.lba = fat_to_lba(cluster);
            fs->file.pos = 0;
            fs->file.entry = entry;
            ret = 0;
            goto out;
        }
    }

out:
    return ret;
}

i32t fat_read(void *buff, u32t len)
{   
    struct fat_inode *fil = &fs->file;
    u8t *out = (u8t*)buff;
    u32t ret = 0;

    if (!fs || !fs_dev || !fs->file.opened)
        return SIGN(EFAULT);

    if (!buff || !len)
        return SIGN(EFAULT);
    
    /* Check if file position not exceed reported by FS */
    if (fs->file.pos >= fs->file.entry.size)
        return 0; /* EOF */
    
    if (fs->file.pos + len > fs->file.entry.size)
        len = fs->file.entry.size - fs->file.pos; 

    /* obtain end of chain */
    u32t eof = (fs->type == FAT32_FS_IDENT) ? FAT32_EOF : (fs->type == FAT16_FS_IDENT) ?
               FAT16_EOF : FAT12_EOF;
                       
    while (len > 0) {
        u32t sector_offset = u64_mod(u64_div(fs->file.pos, fs->b.info.sector_size), fs->b.info.cluster_size);
        u32t offset = u64_mod(fs->file.pos, fs->b.info.sector_size);
        u32t lba = fat_to_lba(fil->current_cluster) + sector_offset;

        if (PART_READ(fs_dev, lba, 1, fil->buffer) < 0)
            break;

        u32t take = min(len, fs->b.info.sector_size - offset);
        memcpy(out, fil->buffer + offset, take);

        out          += take;
        fs->file.pos += take;
        ret          += take;
        len          -= take;

        if (!u64_mod(u64_div(fs->file.pos, fs->b.info.sector_size), fs->b.info.cluster_size)) {
            u16t next = fat_next_cluster(fil->current_cluster);
            if (next >= eof || next == 0x000)
                break; /* read finished */
            fil->current_cluster = next;
        }
    }
    
    /* return bytes readed */
    return ret;
}

int fat_close()
{
    fs->file.pos = 0;
    fs->file.opened = false;
    return 0;
}