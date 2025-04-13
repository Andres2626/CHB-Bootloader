
/*
* entry.c -- stage2 begin function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/disk.h>
#include <CHB/fs/fat.h>
#include <CHB/mm.h>
#include <CHB/port.h>
#include <CHB/types.h>
#include <CHB/video.h>

#include <CHB/ctype.h>
#include <CHB/errno.h>
#include <CHB/minmax.h>
#include <CHB/stdio.h>
#include <CHB/string.h>

static uint8_t* kern_buff = (uint8_t)0x30000;
static uint32_t kern_load_size = 0x10000; /* 128 sectors at maximum */
typedef void (*kern_jmp)();

static void
error(int error) {
   /* TODO: Rework this! */
   printf("CHB error %i: %s\n", error, strerror(error));
}

/* video cursor colors */
void
entry(uint8_t drive_number) {
   int ret = 0;
   struct device disk = {drive_number, 0, 0, 0, 0};

   /* initialize video system */
   video_driver_init(0x3);

   /* print welcome message */
   printf("Loading CHB. Please Wait...\n");

   ret = devinit(&disk);
   if (ret != ENO) {
      error(ret);
      goto end;
   }

   /* Initialize FAT12 driver */
   ret = fat_init(&disk);
   if (ret != ENO) {
      error(ret);
      goto end;
   }

   /* This part of code loads RAW kernel in the memory without
  pushing arguments. 
  
  TODO: * Fully support for VFS 
        * Executable formats
  	  * Other kernel boot methods (Multiboot, linux, BSD, partition, ...)
  	  * Rework this!
  */

   /* load the kernel in the memory */
   struct FAT_file* fp = fat_open(&disk, "/kernel.bin");
   if (!fp) {
      printf("Error opening file /kernel.bin\n");
      goto end;
   }

   uint32_t read;
   uint8_t* buff = kern_buff;
   read = fat_read(&disk, fp, kern_load_size, kern_buff);
   while (read) {
      memcpy(buff, kern_buff, read);
      buff += read;
   }
   fat_close(fp);

   /* jump to kernel */
   kern_jmp kern = (kern_jmp)kern_buff;
   kern();

end:
   for (;;)
      ;
}