/*
* mkbs12.c -- Make fat12 CHB bootsector
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

/* bassed on mkfs.fat */

/* 
* For use this program you should have created FAT image via mkfs.fat,
* specifying at least 45 reserved sectors (option -R), these sectors are used for CHB to save stage 2.
* The program changes the machine code given by mkfs.fat and changes it to the 
* CHB code, which means that it changes the instruction at the beginning so 
* that the program jumps to 0x3C and the bootsector leaves it intact and rewrites 
* the remaining bytes up to 510
* NOTES!: 
* - The bootloader by default jmp to 0x3C but can change, So be careful when editing the BOOT_BPB_END 
*   definition in stage1/stage1.h
* - CHB requires requires the input image to have at least 45 sectors to store stage 2, Therefore the 
*   number of reserved sectors must be at least 45
* - The image must have the number of sectors reported by the mkfloppy program 
*  (720, 1440, 2880 and 5760 sectors) according to INT13,8
*/

#include <fs/fat.h>
#include <stage1/stage1.h>

#include "types.h"

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

#define PROG_NAME "mkbs"
#define PROG_VER  "1.0"

const char* argp_program_version = "CHB mkbs 1.0";
static char* input = NULL;
static char* prefix = NULL;

union FAT_bs {
   struct fat_boot_sector bs;
   u8t data[512];
};

static int parse_opt(int key, char *arg, __attribute__((unused)) struct argp_state *state) 
{
   switch (key) {
      case 'i': input = arg; break;
      case 'p': prefix = arg; break;
   }
   return 0;
}

struct argp_option options[] = {
    {"input-fat-img", 'i', "FILE", 0,
     "Input formated FAT12 image. This image is edited and at the end it remains with the bootsector of CHB and the "
     "BPB of FAT."},
    {"prefix", 'p', "DIR", 0, "Directory where CHB was installed. The program looks for stage1.IMG "},
    {0}};
struct argp argp = {options, parse_opt};

int main(int argc, char **argv) 
{
   FILE *stage1_img, *fat_img;
   char *buff = malloc(255 * sizeof(char));
   union FAT_bs stage1_bs, input_bs;
   size_t size;

   argp_parse(&argp, argc, argv, 0, 0, 0);

   if (!prefix) {
      printf("CHB directory is not defined. try with --prefix=[DIR] option\n");
      return 1;
   }

   if (!input) {
      printf("Input file is not defined. try with --input-fat-img=[FILE] option\n");
      return 1;
   }

   sprintf(buff, "%s/stage1.IMG", prefix);
   stage1_img = fopen(buff, "rb");

   if (!stage1_img) {
      printf("Error opening CHB stage1 image.\n");
      fclose(stage1_img);
      return 1;
   }

   free(buff);

   fread(&stage1_bs, 512, 1, stage1_img);
   fclose(stage1_img);

   fat_img = fopen(input, "rb");
   if (!fat_img) {
      printf("Error opening input image.\n");
      fclose(fat_img);
      return 1;
   }
   fseek(fat_img, 0, SEEK_END);
   size = ftell(fat_img);
   fseek(fat_img, 0, SEEK_SET);
   fread(&input_bs, 512, 1, fat_img);
   fclose(fat_img);

   /* check if image have correct sector size */
   size_t sec_size = (size + 511 >> 9); /* avoids truncated int */
   switch (sec_size) {
      case 720:
      case 1440:
      case 2880:
      case 5760: break;
      default: printf("Invalid image size. %i sectors: %i\n", size, sec_size); return 1;
   }

   /* detect valid JMP */
   if (input_bs.bs.jmp[1] != BOOT_BPB_END) {
      printf("Invalid JMP instruction. 0x%x\n", input_bs.bs.jmp[1]);
      return 1;
   }

   /* check if the image have enough space for save stage2 */
   if (input_bs.bs.reserved_sectors < 45) {
      printf("Invalid FAT reserved sectors. %i\n", input_bs.bs.reserved_sectors);
      return 1;
   }

   /* check if BPB have enough space for copy FAT BPB */
   if (input_bs.bs.jmp[1] < 0x3C) {
      printf("Invalid BPB size.0x%x\n", input_bs.bs.jmp[1]);
      return 1;
   }

   /* detect valid BIOS signature */
   if (input_bs.data[510] != 0x55 || input_bs.data[511] != 0xaa) {
      printf("Invalid bootsector signature.\n");
      return 0;
   }

   /* Copy CHB code to input image */
   fat_img = fopen(input, "r+");
   if (!fat_img) {
      printf("Error opening input image.\n");
      fclose(fat_img);
      return 1;
   }

   for (int i = BOOT_BPB_END; i < 510; i++) {
      input_bs.data[i] = stage1_bs.data[i];
   }

   fseek(fat_img, 0, SEEK_SET);
   fwrite(&input_bs, 512, 1, fat_img);
   fclose(fat_img);
   printf("copied CHB code into input image.\n");
   return 0;
}