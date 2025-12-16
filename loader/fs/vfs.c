
/*
* vfs.c -- VFS controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <fs/fs.h>

#include "lib/libc/ctype.h"
#include "lib/libc/errno.h"
#include "lib/libc/stdio.h"
#include "lib/libc/string.h"

PRIVATE struct fs_ops *vfs_reg_tab[MAX_FS];
PRIVATE struct fs *vfs_mount_list[MAX_MOUNT_POINTS];
PRIVATE struct file vfs_opened_list[MAX_FILES_OPENED];

int vfs_register_fs(struct fs_ops *fs) 
{ 
    /*
     * This function register FS in 'special' table, find first
     * 'NULL' entry and save FS descriptor here.
     */
    int i; /* FS register index */ 
    
    if (!fs)
        return SIGN(EFAULT);
    
    for (i = 0; i < MAX_FS; i++) { 
        if (!vfs_reg_tab[i])
            goto table_scan_success;
    }
    
    return SIGN(EREGFS);
table_scan_success:
    vfs_reg_tab[i] = fs; 
    return EIO; 
}

struct fs *vfs_detect(struct device *disk)
{
    PRIVATE struct fs_ops *ops; /* FS functions */
    PRIVATE struct fs *fs;      /* FS descriptor */
    
    if (!disk)
        return NULL;
    
    for (int i = 0; i < MAX_FS; i++) {
        ops = vfs_reg_tab[i];
        if (!ops)
            continue;

        fs = ops->detect(disk);
        if (fs)
            goto success;
    }

    return NULL;
success:
    fs->ops = ops;
    return fs;
}

int vfs_mount(struct fs *fs)
{
    int i; /* FS mount index */

    if (!fs || !fs->ops || !fs->fs_dev)
        return SIGN(EINVAL);

    if (!fs->ops->mount(fs->fs_dev))
        return SIGN(EFAULT);

    for (i = 0; i < MAX_MOUNT_POINTS; i++) {
        if (!vfs_mount_list[i])
            break;
    }

    if (i == MAX_MOUNT_POINTS)
        return SIGN(EREGFS);

    vfs_mount_list[i] = fs;

    return EIO;
}

int vfs_unmount(struct fs *fs)
{
    int i; /* FS mount index */

    if (!fs || !fs->ops || !fs->fs_dev)
        return SIGN(EINVAL);
    
    for (i = 0; i < MAX_MOUNT_POINTS; i++) {
        if (vfs_mount_list[i] == fs)
            goto success;
    }
    
    if (i == MAX_MOUNT_POINTS)
        return SIGN(EREGFS);
    
success:
    /* unmount internal FS */
    fs->ops->unmount();
    vfs_mount_list[i] = NULL;
    return EIO;
}

struct file *vfs_open(_CONST char *path)
{
    /* 
     * This is the path format '/mount_ident/path/to/file.txt'
     * mount identifier valid values: 'A - Z' (not case-sensitive)
     */
 
    int vol;            /* volume index */
    int i;              /* file index */
    int j;              /* buffer index */
    char mpoint;        /* mount ident */
    char buff[255];     /* path array */
    struct file *fp;    /* file descriptor */
    struct fs *fs;      /* FS descriptor */
    
    if (!path)
        goto fail;
    
    /* ignore path[0] if is '/' */
    if (*path == '/')
        path++;
    
    /* set mount point identifier */
    mpoint = *path;
    
    /* check if the 'ident' is valid */
    if ((mpoint >= 'a' && mpoint <= 'z') || (mpoint >= 'A' && mpoint <= 'Z'))
        mpoint = toupper(mpoint);
    else {
        printf("[FS] Invalid path prefix\n");
        goto fail;
    }
    
    /* get the volume index from ident */
    vol = (char)(mpoint - 'A');
    if (vol < 0 || vol >= MAX_MOUNT_POINTS) {
        printf("[FS] Invalid volume prefix\n");
        goto fail;
    }
    path++;
    
    if (*path != '/') {
        printf("[FS] Invalid path prefix\n");
        goto fail;
    }
    path++;
    
    /* copy remaining path to buffer */
    j = 0;
    while (*path != '\0' && j < (int)sizeof(buff) - 1) {
        buff[j++] = *path++;
    }
    buff[j] = '\0';
    
    /* check if volume is mounted */
    fs = vfs_mount_list[vol];
    if (!fs || !fs->ops || !fs->ops->open) {
        printf("[FS] '%c' volume doesn't exists in mounted devices\n", mpoint);
        goto fail;
    }
    
    for (i = 0; i < MAX_FILES_OPENED; i++) {
        if (!vfs_opened_list[i].opened) {
            fp = &vfs_opened_list[i];
            memset(fp, 0, sizeof(struct file));
            goto end_table_scan;
        }
    }
    goto fail;
    
end_table_scan:
    fp->opened = true;
    fp->pos = 0;
    fp->inode_number = i;
    
    /* pass remaining path to FS driver */
    if (fs->ops->open(fp, buff) < 0)
        goto fail;
    
    fp->fs = fs;
    
    return fp;
fail:
    return NULL;
}

i32t vfs_read(struct file *fp, void *buff, u32t len)
{
    if (!fp || !fp->fs || !fp->fs->ops || !fp->fs->ops->read)
        return SIGN(EINVAL);
    
    if (!buff || !len)
        return SIGN(EINVAL);
    
    return fp->fs->ops->read(fp, buff, len);
}

int vfs_close(struct file *fp)
{
    if (!fp)
        return SIGN(EINVAL);
    
    /* delete file from array */
    for (int i = 0; i < MAX_FILES_OPENED; i++) {
        if (fp->inode_number == i && fp->opened) {
            memset(fp, 0, sizeof(struct file));
            goto success;
        }
    }
    
success:
    fp = NULL;
    return 0;
}