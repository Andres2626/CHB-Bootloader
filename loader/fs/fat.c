
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
#include "lib/libc/stdlib.h"

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

struct fat_inode {
    bool opened;
    bool dir;
    bool root;
    u32t lba;
    u32t first_cluster;
    u32t current_cluster;
    u32t current_sector;
    struct file f;
    struct fat_entry entry;
    u8t buffer[FAT_SECSIZE];
};

struct fat_fs {    
    union {
        struct fat_boot_sector info;
        u8t buffer[FAT_SECSIZE];
    } b;
    
    struct fat_inode root;
    
    struct {
        u32t lba;
        u32t size;
    } data;
    
    struct fat_inode opened[MAX_FILES_OPENED];
};

PRIVATE struct device *fs_dev;
PRIVATE struct fat_fs *fs; /* FS info */
PRIVATE u8t *fs_tab; /* The file allocation table */
PRIVATE struct fs f;

PRIVATE int read_bsec()
{
    if (!fs_dev)
        return SIGN(EFAULT); /* fail */
    
    return devread(fs_dev, 0, 1, fs->b.buffer);
}

PRIVATE int read_fat()
{
    if (!fs_dev)
        return SIGN(EFAULT); /* fail */
    
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
        return SIGN(EINVAL);
    
    if (!fs_dev)
        return SIGN(EFAULT);
    
    /* set root info */
    fs->root.root = true;
    fs->root.dir = true;
    fs->root.f.pos = 0;
    fs->root.f.inode.dir = true;
    fs->root.f.inode.size = size;
    fs->root.lba = begin + sector;
    fs->root.first_cluster = 0;
    fs->root.current_cluster = 0;
    fs->root.current_sector = sector;
    
    /* read root */
    return devread(fs_dev, fs->root.lba, 1, fs->root.buffer);
}

PRIVATE u32t fat_to_lba(u32t cluster) 
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
    fs_dev = disk;
    fs = (struct fat_fs*)FS_DRIVER_DATA;
    
    if (!fs_dev || fs_dev->state != DRIVE_OK)
        goto fail;

    
    if (!read_bsec())
        goto fail;
    
    /* check valid FAT format */
    u16t sig = (fs->b.buffer[511] << 8) | fs->b.buffer[510];
    
    if (sig == FAT_MAGIC)
        goto success;
    
fail:
    return NULL;
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
    
    if (!fs)
        return SIGN(EFAULT);
    
    if (!fs_dev || fs_dev->state != DRIVE_OK)
        return SIGN(EIO);
    
    /* FAT driver only accepts multiples of 512 in sector size */
    if ((fs->b.info.sector_size % 512) != 0) 
        return SIGN(EIFS);
    
    fat_size = fs->b.info.FAT_size * fs->b.info.sector_size;
    if ((sizeof(struct fat_fs) + fat_size) > FS_DRIVER_DATA_SIZE)
        return SIGN(ENOMEM);
    
    int ret = read_fat();
    if (ret < 0)
        goto fail;
    
    ret = read_root(0);
    if (ret < 0)
        goto fail;
    
    /* get data info */
    fs->data.size = (fs->b.info.dir_entries * 32 + fs->b.info.sector_size - 1)  / fs->b.info.sector_size;
    fs->data.lba = fs->b.info.reserved_sectors + fs->b.info.FAT_count * fs->b.info.FAT_size + fs->data.size;
    return EIO;
fail:
    return ret;
}

PUBLIC void fat_unmount()
{
    fs = NULL;
    fs_dev = NULL;
    fs_tab = NULL;
}

int fat_open(struct file *fp, _CONST char *path)
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
    
    /* tokenize path */
    char *tok = strtok(tmp, "/");
    if (!tok) 
        return SIGN(EFAULT); /* invalid token */
    
    int ret = 0;
    while (tok) {
        char *fatname = fat_path_to_dos(tok);
        found = false;

        /* check if current dir is root dir TODO: this not work in FAT32 */
        if (cur.root) {
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
                        break;
                    }
                }
                
                if (found)
                    break;
            }     
        }
        else {
            u16t cluster = cur.first_cluster;
            while (cluster < FAT_EOF12) {
                u32t lba = fat_to_lba(cluster);
                
                for (u32t s = 0; s < fs->b.info.cluster_size; s++) {
                    ret = devread(fs_dev, lba + s, 1, cur.buffer);
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
        if (!found) {
            ret = -EFILE;
            goto out;
        }

        tok = strtok(NULL, "/");
        if (tok) {
            
            /* check if entry is 'file' */
            if (!(entry.attributes & FAT_DIR)) {
                ret = -EIFS;
                goto out;
            }
            
            /* reload internal file descriptor */
            cur.first_cluster = entry.cluster_lo;
            cur.lba = fat_to_lba(entry.cluster_lo);
            cur.f.inode.dir = true;
            cur.root = false;
        }
        else
        {
            /* push FAT info */
            struct fat_inode *fi = &fs->opened[fp->inode_number];
            fi->opened = true;
            fi->dir = false;
            fi->root = false;
            fi->first_cluster = entry.cluster_lo;
            fi->current_cluster = entry.cluster_lo;
            fi->current_sector = 0;
            fi->lba = fat_to_lba(entry.cluster_lo);
            fi->f.inode.size = entry.size;
            fi->f.pos = 0;
            fi->entry = entry;
            
            
            /* push file descriptor to 'VFS' */
            fp->inode.dir = false;
            fp->inode.size = entry.size;
            fp->pos = 0;
            
            ret = 0;
            goto out;
        }
    }

out:
    return ret;
}

i32t fat_read(struct file *fp, void *buff, u32t len)
{   
    if (!fp || !fp->fs)
        return SIGN(EFAULT);
    
    if (!buff || !len)
        return SIGN(EFAULT);
    
    int ninode = fp->inode_number;
    struct fat_inode *fil = &fs->opened[ninode];
    u8t *out = (u8t*)buff;
    u32t bytes_read = 0;
    
    if (fp->pos >= fil->f.inode.size)
        return SIGN(EIFS);

    if (fp->pos + len > fil->f.inode.size)
        len = fil->f.inode.size - fp->pos;
    
    while (len > 0) {
        u32t sector_offset = u64_mod(u64_div(fp->pos, fs->b.info.sector_size), fs->b.info.cluster_size);
        u32t offset = u64_mod(fp->pos, fs->b.info.sector_size);
        u32t lba = fat_to_lba(fil->current_cluster) + sector_offset;

        if (devread(fs_dev, lba, 1, fil->buffer) < 0)
            break;

        u32t take = min(len, fs->b.info.sector_size - offset);
        memcpy(out, fil->buffer + offset, take);

        out        += take;
        fp->pos    += take;
        bytes_read += take;
        len        -= take;

        if (!u64_mod(u64_div(fp->pos, fs->b.info.sector_size), fs->b.info.cluster_size)) {
            u16t next = fat_next_cluster(fil->current_cluster);
            if (next >= FAT_EOF12 || next == 0x000)
                break;
            fil->current_cluster = next;
        }
    }

    return bytes_read;
}

int fat_close(struct file *fp)
{
    if (!fp)
        return SIGN(EFAULT);
    
    /* delete file from array */
    for (int i = 0; i < MAX_FILES_OPENED; i++) {
        if (fp->inode_number == i) {
            memset(&fs->opened[fp->inode_number], 0, sizeof(struct fat_inode));
            break;
        }
    }
    
    return 0;
}