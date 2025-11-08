
/*
* entry.c -- stage2 begin function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <fs/fs.h>
#include <fs/fat.h>
#include <sys/disk.h>
#include <sys/mm.h>
#include <sys/port.h>
#include <sys/video.h>

#include "const.h"
#include "types.h"

#include "lib/libc/ctype.h"
#include "lib/libc/errno.h"
#include "lib/libc/minmax.h"
#include "lib/libc/stdio.h"
#include "lib/libc/string.h"

#define FS_FAT 0

struct msys_kern_boot {
	struct device disk; /* disk properties */
	struct memory_info mem; /* memory info */
	u8t vid_mode; /* video BIOS mode */
};

PRIVATE u8t *kernel = (u8t*)KERN_LOC; 
//PRIVATE u8t *kernel_load = (u8t*)KERN_BUFFER;
PRIVATE struct fs_ops fat12_ops = {
    .name   = "fat12",
    .detect = fat_detect_fs,
    .mount  = fat_mount,
    .unmount = fat_unmount,
    .open   = fat_open,
};

typedef void (*kern_jmp)(struct msys_kern_boot *header) __attribute__((regparm(1)));

PRIVATE void perror() 
{
    if (!errno)
        return;
    
    printf("\nCHB error %i: %s.\n", errno, strerror(errno));
    for (;;);
}

PRIVATE int fs_init() 
{
    printf("[FS] Initializing FS\n");
    return vfs_register_fs(&fat12_ops);
}

/* video cursor colors */
void entry(u8t drive_number) 
{
    struct device disk = {drive_number, 0, 0, 0, 0};
    struct video video = { 0x3, BLACK, LIGHT_GRAY, { true, 15, 16, 0, 0, 0 }};
    struct memory_info mem;
	
    /* initialize video system */
    video_init(&video);

    /* print welcome message */
    printf("[LOADER] Loading CHB. Please Wait...\n");
	
	if (!memory_init(&mem))
		perror();
	
    if (!devinit(&disk)) 
        perror();
    
    if (!fs_init())
        perror();
    
    struct fs *fd = vfs_detect(&disk);
    if (!fd)
        perror();
    
    if (!vfs_mount(fd))
        perror();
    
    /* check if kernel is located afther fisrt 1MB of memory. */
    if ((void*)kernel < (void*)KERN_LOC) {
	    errno = EKERNLOC;
	    perror();
    }
    
    /* load the kernel in the memory */
    struct file *fp = vfs_open("a/file.txt");
    if (!fp) {
        printf("[LOADER] a/kernel.bin not found.\n");
        errno = EKERN;
        perror();
    }
    
    goto loop;

#if 0
   
    /* load kernel in memory */
    printf("[LOADER] Loading kernel...\n");

    u32t read;
    u8t *kern_buff = kernel;
    while ((read = fs_table[FS_FAT].read(fp, KERN_MSIZE, kernel_load))) {
	    memcpy(kern_buff, kernel_load, read);
	    kern_buff += read;
    }
    fs_table[FS_FAT].close(fp);
   
    /* bootstrap kernel */
    printf("\nBooting kernel at %p.\n", kernel);
	
	struct msys_kern_boot header;
	header.disk = disk;
	header.mem = mem;
	header.vid_mode = video.mode;
	
    kern_jmp kernel_start = (kern_jmp)kernel;
    kernel_start(&header);
    
#endif

loop:
    for (;;);
}