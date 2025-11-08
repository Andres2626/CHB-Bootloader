
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
PRIVATE struct file *vfs_opened_list[MAX_FILES_OPENED];

int vfs_register_fs(struct fs_ops *fs) 
{ 
    /*
     * This function register FS in 'special' table, find first
     * 'NULL' entry and save FS descriptor here.
     */
    int i; /* FS register index */ 
    
    if (!fs)
        goto fail;
    
    for (i = 0; i < MAX_FS; i++) { 
        if (!vfs_reg_tab[i])
            goto table_scan_success;
    }
    
fail:
    errno = EREGFS; 
    return 0;
table_scan_success:
    vfs_reg_tab[i] = fs; 
    return 1; 
}

struct fs *vfs_detect(struct device *disk)
{
    PRIVATE struct fs_ops *ops; /* FS functions */
    PRIVATE struct fs *fs;      /* FS descriptor */
    
    if (!disk) {
        errno = EDEVINT;
        goto fail;
    }
    
    for (int i = 0; i < MAX_FS; i++) {
        ops = vfs_reg_tab[i];
        if (!ops)
            continue;

        fs = ops->detect(disk);
        if (fs)
            goto success;
    }

    errno = EIFS;
success:
    fs->ops = ops;
    return fs;
fail:
    return NULL;
}

int vfs_mount(struct fs *fs)
{
    int i; /* FS mount index */

    if (!fs || !fs->ops || !fs->fs_dev)
        goto fail;

    if (!fs->ops->mount(fs->fs_dev))
        goto fail;

    for (i = 0; i < MAX_MOUNT_POINTS; i++) {
        if (!vfs_mount_list[i])
            break;
    }

    if (i == MAX_MOUNT_POINTS)
        goto fail;

    vfs_mount_list[i] = fs;

    return 1;
fail:
    return 0;
}

int vfs_unmount(struct fs *fs)
{
    int i; /* FS mount index */

    if (!fs || !fs->ops || !fs->fs_dev)
        goto fail;
    
    for (i = 0; i < MAX_MOUNT_POINTS; i++) {
        if (vfs_mount_list[i] == fs)
            goto success;
    }

success:
    /* unmount internal FS */
    fs->ops->unmount();
    vfs_mount_list[i] = NULL;
    return 1;
fail:
    return 0;
}

PUBLIC struct file *vfs_open(_CONST char *path)
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
        printf("[FS] Invalid path prefix");
        goto fail;
    }
    
    /* get the volume index from ident */
    vol = (char)(mpoint - 'A');
    if (vol < 0 || vol >= MAX_MOUNT_POINTS) {
        printf("[FS] Invalid volume prefix");
        goto fail;
    }
    path++;
    
    if (*path != '/') {
        printf("[FS] Invalid path prefix");
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
    
    /* pass remaining path to FS driver */
    fp = fs->ops->open(buff);
    if (!fp)
        goto fail;
    
    fp->fs->ops = fs->ops;

    for (i = 0; i < MAX_FILES_OPENED; i++) {
        if (!vfs_opened_list[i])
            break;
    }
    
    if (i >= MAX_FILES_OPENED) {
        errno = EMFILE;
        goto fail;
    }
    
    /* push file descriptor in file array */
    vfs_opened_list[i] = fp;
    
    return fp;
fail:
    return NULL;
}

int vfs_close(struct file *fp)
{
    int i; /* file index */
    
    if (!fp)
        goto fail;
    
    /* delete file from array */
    for (i = 0; i < MAX_FILES_OPENED; i++) {
        if (vfs_opened_list[i] == fp)
            goto success;
    }
    
success:
    /* clean file manager */
    vfs_opened_list[i] = NULL;
    memset(fp, 0, sizeof(struct file));
    return 1;
fail:
    return 0;
}