
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

uint8_t *kernel = (uint8_t*)0x100000; 
uint8_t *kernel_load = (uint8_t*)0x30000;

#define KERN_MAX_SIZE 0x10000
typedef void (*kern_jmp)();

extern int get_lower_memory();
extern int get_upper_memory();

static void
perror() {
   printf("\nCHB error %i: %s.\n", errno, strerror(errno));
}

/* video cursor colors */
void
entry(uint8_t drive_number) {
   struct device disk = {drive_number, 0, 0, 0, 0};

   /* initialize video system */
   video_driver_init(0x3);

   /* print welcome message */
   printf("Loading CHB. Please Wait...\n\n");
	
   if (!devinit(&disk)) {
      perror();
      goto end;
   }

   /* Initialize FAT12 driver */
   if (!fat_init(&disk)) {
      perror();
      goto end;
   }
   
   printf("Lower: %iK\n", get_lower_memory());
   printf("Upper: %iK\n", get_upper_memory());
   
   /* This part of code loads RAW kernel in the memory without
  pushing arguments. 
  
  TODO: * Fully support for VFS 
        * Executable formats
  	    * Other kernel boot methods (Multiboot, linux, BSD, partition, ...)
  	    * Rework this!
  */
   
   /* Check if kernel is located afther fisrt 1MB of memory. */
   if ((void*)kernel < (void*)KERNEL_LOCALIZATION) {
	   set_errno(ERR_KERN_LOC);
	   perror();
	   goto end;
   }
   
   /* load the kernel in the memory */
   struct FAT_file* fp = fat_open(&disk, "/kernel.bin");
   if (!fp) {
	  printf("/KERNEL.BIN not found.\n");
	  set_errno(ERR_KERN_NO);
      perror();
      goto end;
   }
   
   /* load kernel in memory */
   printf("Loading kernel...\n");
   uint32_t read;
   uint8_t *kern_buff = kernel;
   while ((read = fat_read(&disk, fp, KERN_MAX_SIZE, kernel_load))) {
	   memcpy(kern_buff, kernel_load, read);
	   kern_buff += read;
   }
   fat_close(fp);
   
   /* bootstrap kernel */
   printf("Booting kernel at %p.\n", kernel);
   kern_jmp kernel_start = (kern_jmp)kernel;
   kernel_start();
  
end:
   for (;;)
      ;
}