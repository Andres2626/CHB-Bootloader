
/*
* fatinfo.c -- view bootsector of FAT12 FS filesystem
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <fs/fat.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PROG_NAME "fatinfo"
#define PROG_VER  "1.0"

union FAT_bs {
   struct fat_boot_sector bs;
   u8t data[512];
};

void usage() 
{
   fprintf(stderr, "CHB %s %s\n"
           "usage: %s <input_file>\n",
           PROG_NAME, PROG_VER, PROG_NAME);
}

#define BS bs.bs

int main(int argc, char** argv) 
{
   FILE* fp;
   union FAT_bs bs;
   char* input = argv[1];

   if (argc < 2) {
      printf("no argument specified.\n");
      usage();
      return 1;
   }

   if (!input) {
      printf("input file not specified.\n");
      return 1;
   }

   fp = fopen(input, "rb");
   if (!fp) {
      printf("error reading input file. %s\n", input);
      return 0;
   }

   /* read first sector of image */
   fseek(fp, 0, SEEK_SET);
   if (!fread(bs.data, 512, 1, fp)) {
      printf("error obtaining bootsector.\n");
      return 0;
   }

   /* check if image is a valid FAT filesystem */
   if (bs.data[510] != 0x55 || bs.data[511] != 0xaa) {
      printf("Invalid bootsector signature.\n");
      return 0;
   }

   if (BS.jmp[1] < 0x3C) {
      printf("Invalid BPB size (%i)", BS.jmp[1]);
      return 0;
   }

   /* print bootsector */
   printf("asm instruction: 0x%x 0x%x 0x%x\n", BS.jmp[0], BS.jmp[1], BS.jmp[2]);
   printf("asm instruction (valid): %s\n",
          (BS.jmp[0] == 0xEB && BS.jmp[2] == 0x90) ? "valid" : "invalid expected 0xEB X 0x90");
   printf("oem: %s\n", BS.oem);
   printf("sector size: %i\n", BS.sector_size);
   printf("cluster size: %i\n", BS.cluster_size);
   printf("reserved size: %i\n", BS.reserved_sectors);
   printf("FATS: %i\n", BS.FAT_count);
   printf("entries: %i\n", BS.dir_entries);
   printf("Drive size: %i\n", BS.drive_size);
   printf("FAT size: %i\n", BS.FAT_size);
   printf("track size: %i\n", BS.track_size);
   printf("heads: %i\n", BS.drive_heads);
   printf("hidden_sectors: %i\n", BS.hidden_sectors);
   printf("large_sectors: %i\n", BS.large_sectors);
   printf("EBR:\n");
   printf("drive number: %i\n", BS.drive_number);
   printf("reserved: %i\n", BS.reserved);
   printf("signature: %i\n", BS.signature);
   printf("volume id: 0x%x\n", BS.volumeid);
   printf("label: %s\n", BS.label);
   printf("id: %s\n", BS.id);
}