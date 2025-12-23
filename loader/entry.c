
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
#include <sys/exec.h>
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

#define MSYS_KERN_MAGIC 0x3F8A857B

struct msys_kern_hdr {
    u32t magic;
	struct device disk; /* disk properties */
	struct memory_info mem; /* memory info */
	u8t vid_mode; /* video BIOS mode */
};

PRIVATE u8t *kernel_load = (u8t*)KERN_BUFFER;
PRIVATE struct fs_ops fat12_ops = {
    .name   = "fat12",
    .detect = fat_detect_fs,
    .mount  = fat_mount,
    .unmount = fat_unmount,
    .open   = fat_open,
    .read = fat_read
};

typedef void (*kern_jmp)(struct msys_kern_hdr *header) __attribute__((regparm(1)));

PRIVATE void error(int code) 
{   
    printf("\nCHB error %i: %s.\n", -code, strerror(-code));
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
    struct device disk;
    struct video video = { 0x3, BLACK, LIGHT_GRAY, { true, 15, 16, 0, 0, 0 }};
    struct memory_info mem;
    int ret = 0;
    
    disk.number = drive_number;
    disk.state = 0;
	
    /* initialize video system */
    video_init(&video);

    /* print welcome message */
    printf("[LOADER] Loading CHB. Please Wait...\n");
	
    printf("[LOADER] Initializing memory\n");
    ret = memory_init(&mem);
	if (ret < 0)
		error(ret);
    
    printf("[MM] lower: %iK, upper: %iK\n", mem.lo, mem.hi);
	for (int i = 0; i < mem.count; i++)
        printf("[MM] block, begin: 0x%llx, len: 0x%llx, type: %li, flag: 0x%lx\n", 
	           mem.blk[i].begin, mem.blk[i].len, mem.blk[i].type, mem.blk[i].flag);
    
    printf("[LOADER] Initializing disk\n");
    ret = devinit(&disk);
    if (ret < 0) 
        error(ret);
    
    printf("[LOADER] Initializing FS\n");
    ret = fs_init();
    if (ret < 0)
        error(ret);
    
    struct fs *fd = vfs_detect(&disk);
    if (!fd)
        error(SIGN(EFS));
    
    printf("[LOADER] Current FS name: %s, disk: 0x%x\n", fd->ops->name, drive_number);
    
    ret = vfs_mount(fd);
    if (ret < 0)
        error(ret);
    
    /* load the kernel in the memory */
    struct file *fp = vfs_open("a/kernel.elf");
    if (!fp) {
        printf("[LOADER] a/kernel.elf not found.\n");
        error(SIGN(EKERN));
    }
    
    /* load kernel in memory */
    printf("[LOADER] Loading kernel...\n");
    i32t read = vfs_read(fp, kernel_load, KERN_SIZE);
    if (read < 0)
        error(ret);
    
    vfs_close(fp);
    
    void *entry;
    ret = elf_load(kernel_load, KERN_BUFFER, &entry);
    if (ret < 0) {
        printf("[LOADER] Kernel file invalid\n");
        error(ret);
    } 
    
    if (entry < (void*)KERN_LOC) 
        error(SIGN(EKERNLOC));

    /* bootstrap kernel */
    printf("\nBooting kernel at %p.\n", entry);
	
	struct msys_kern_hdr hdr;
    hdr.magic = MSYS_KERN_MAGIC;
	hdr.disk = disk;
	hdr.mem = mem;
	hdr.vid_mode = video.mode;
	
    kern_jmp kernel_start = (kern_jmp)entry;
    kernel_start(&hdr);
    
    goto loop;

loop:
    for (;;);
}