
/* copychb.c -- program for copy CHB bootcode into FAT12/16 image */

#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fat.h"

#define SECTOR_SIZE 512
#define FAT12 12
#define FAT16 16
#define FAT32 32

#define PROGRAM_NAME "copychb"
#define PROGRAM_VERSION "0.1 alpha"

const char* prefix = "./";

static struct option long_options[] = {
    {"print-bootsector", required_argument, 0, 'b'},
    {"help", no_argument, 0, 'h'},
    {"make-chb-image", required_argument, 0, 'm'},
    {"prefix", required_argument, 0, 'p'},
    {"usage", no_argument, 0, 'u'},
    {0, 0, 0, 0}
};

void usage(const char* program_name, int eval) {
    fprintf(stderr, "\
Usage: %s [OPTIONS] [TARGET]\n\
\n\
Create a bootable FAT12/16 image with CHB bootsector.\n\
only works on formated images, tthe program does'nt format images\n\
To format images with FAT 12/16 you need to do it from mkfs.fat or\n\
similar and reserve beetween 24 and 32 sectors because CHB put \n\
the stage1 afther to first LBA sector of your floppy.\n\
\n\
Options: \n\
  -b [IMG] --print-bootsector=[IMG]      Print bootsector of your FAT12/16/32 formated image.\n\
  -h --help                              Print this.\n\
  -m [IMG] --make-chb-image=[IMG]        Put bootsector code of CHB image in your image.\n\
  -p [DIR] --prefix=[DIR]                Set directory of your CHB images are located.\n\
  -u --usage                             Print this.\n", program_name);
    exit(eval);
}

bool read_filesec(FILE* disk, uint8_t sector, uint8_t sector_count, void* data) {
    fseek(disk, sector * SECTOR_SIZE, SEEK_SET); /* set file position */
    return fread(data, SECTOR_SIZE, sector_count, disk) > 0;
}

bool write_filesec(FILE* disk, uint8_t sector, uint8_t sector_count, void* data) {
    fseek(disk, sector * SECTOR_SIZE, SEEK_SET); /* set file position */
    return fwrite(data, SECTOR_SIZE, sector_count, disk) > 0;
}

bool read_bootsector(fat_bootsec_t* bootsector, FILE* disk) {
    /* read the first sector of drive */
    return read_filesec(disk, 0, 1, bootsector);
}

bool write_bootsector(fat_bootsec_t* bootsector, FILE* disk) {
    /* read the first sector of drive *//* read the first sector of drive */
    return write_filesec(disk, 0, 1, bootsector);
}

int determinate_file_len(FILE* disk) {
    fseek(disk, 0L, SEEK_END); /* go to end file */
    int size = ftell(disk);
    size = (size + 511) >> 9; /* convert the size to sector count */
    return size;
}

int determinate_fat_type(fat_bootsec_t* bootsector) {
    uint32_t lba_of_root;
    lba_of_root = bootsector->reserved_sectors + (bootsector->number_of_fats * bootsector->sectors_per_fat); //FAT12/16
    uint32_t data_clusters = (bootsector->total_sectors - lba_of_root) / bootsector->sectors_per_track;
    if (data_clusters < 0xFF5)
        return FAT12;
    else if (bootsector->sectors_per_fat != 0)
        return FAT16;
    else
        return FAT32;
}

void print_bootsector(const char* file) {

    FILE* disk = fopen(file, "rb");
    if (!disk) {
        printf("error reeading file.\n");
        fclose(disk);
        exit(1);
    }

    fat_bootsec_t bootsector;
    if (!read_bootsector(&bootsector, disk)) {
        printf("error getting bootsector.\n");
        fclose(disk);
        exit(1);
    }

    printf("jmp function           ");
    for (int i = 0; i < 3; i++) {
        printf("0x%x ", bootsector.jmp_instruction[i]); /* disassemble main jump function */
    }
    printf("\n");
    printf("OEM name:              %s\n", bootsector.oem_name);
    printf("sector size:           %i bytes\n", bootsector.bytes_per_sector);
    printf("cluster size:          %i sectors\n", bootsector.sectors_per_cluster);
    printf("reserved:              %i sectors\n", bootsector.reserved_sectors);
    printf("fat number:            %i\n", bootsector.number_of_fats);
    printf("root sector:           %i\n", bootsector.root_entry);
    printf("total size:            %i sectors\n", bootsector.total_sectors);
    printf("media:                 %i\n", bootsector.media);
    printf("fat size:              %i sectors\n", bootsector.sectors_per_fat);
    printf("sectors per track:     %i sectors\n", bootsector.sectors_per_track);
    printf("heads:                 %i heads\n", bootsector.heads);
    printf("hidden:                %i sectors\n", bootsector.hidden_sectors);
    printf("total fat32 sectors:   %i sectors\n", bootsector.total_fat32_sectors);

    printf("fat type: ");
    int fat_type = determinate_fat_type(&bootsector);
    if (fat_type == 12)
        printf("12\n");
    else if (fat_type == 16)
        printf("16\n");
    else if (fat_type == 32)
        printf("32\n");
    else
        printf("FAT ERROR TYPE.\n");

    /* print the EBR of FAT12/16 */
    if (fat_type == 12 || fat_type == 16) {
        printf("FAT12/16 extended boot sector:\n");
        printf("drive_number:           0x%x\n", bootsector.fat1x.drive_number);
        printf("reserved:               0x%x\n", bootsector.fat1x.reserved);
        printf("volume_id:              0x%x\n", bootsector.fat1x.volume_id);
        printf("volume name:            ");
        for (int i = 0; i < 11; i++) {
            printf("%c", bootsector.fat1x.volume_name[i]);
        }
        printf("\n");
        printf("fat_name:               ");
        for (int i = 0; i < 11; i++) {
            printf("%c", bootsector.fat1x.fat_name[i]);
        }
        printf("\n");

        printf("%i\n", bootsector.boot_signature);
    } else {
        /* print the EBR of FAT32 */
        printf("FAT32 extended boot sector:\n");
        printf("sectors per fat:            %i sectors\n", bootsector.fat32.sectors_per_fat);
        printf("flags                       %i\n", bootsector.fat32.flags);
        printf("fat version:                %i\n", bootsector.fat32.fat_version_number);
        printf("root:                       %i sectors\n", bootsector.fat32.root_directory_cluster);
        printf("FS info:                    %i sectors\n", bootsector.fat32.fs_info_sector);
        printf("backup:                     %i sectors\n", bootsector.fat32.backup);
        printf("drive_number:               0x%x\n", bootsector.fat32.drive_number);
        printf("reserved:                   0x%x\n", bootsector.fat32.reserved);
        printf("volume_id:                  0x%x\n", bootsector.fat32.volume_id);

        printf("volume name:                ");
        for (int i = 0; i < 11; i++) {
            printf("%c", bootsector.fat32.volume_name[i]);
        }
        printf("\n");

        printf("fat_name:                   ");
        for (int i = 0; i < 11; i++) {
            printf("%c", bootsector.fat32.fat_name[i]);
        }
        printf("\n");
    }

    fclose(disk);
}

void make_chb_image(const char* image_file) {
    /*
     * for use this program you need format the image as
     * FAT (12, 16 or 32), like mkfs.fat
     */
    FILE* image, *boot_image;
    fat_bootsec_t image_bootsec, stage1_bootsec;

    /* open output file */
    image = fopen(image_file, "rb+");
    if (!image) {
        printf("error opening the image. %s.\n", image_file);
        fclose(image);
        exit(1);
    }

    /* get bootsector of input image */
    if (!read_bootsector(&image_bootsec, image)) {
        printf("error getting bootsector.\n");
        exit(1);
    }

    /* open CHB boot image */
    char buffer[256];
    sprintf(buffer, "%sboot.IMG", prefix);
    boot_image = fopen(buffer, "rb");
    if (!boot_image) {
        printf("error opening the CHB boot image. %s.\n", buffer);
        fclose(boot_image);
        fclose(image);
        exit(1);
    }

    /* get bootsector of CHB image */
    if (!read_bootsector(&stage1_bootsec, boot_image)) {
        printf("error getting bootsector.\n");
        fclose(boot_image);
        fclose(image);
        exit(1);
    }

    /*
     * write 'jmp 0x7c5b' to formated image, because CHB boot image
     * have the end of BPB in 0x7c5b for save compatibility beetwen
     * FAT12/16 and FAT32
     */
    strcpy(image_bootsec.jmp_instruction, stage1_bootsec.jmp_instruction);

    /* fill the imag bootsector image with ceroes */
    for (int i = 0; i < FAT1X_BOOT_SIZE; i++) {
        image_bootsec.fat1x.buffer[i] = '\0';
    }

    /* copy the CHB boot code */
    for (int i = 0; i < FAT32_BOOT_SIZE; i++) {
        image_bootsec.fat32.buffer[i] = stage1_bootsec.fat32.buffer[i];
    }

    /* write code to out bootsector */
    write_bootsector(&image_bootsec, image);

    char readbuff[512];
    if (!read_filesec(boot_image, 1, 1, readbuff)) {
        printf("error read second sector of image.\n");
        fclose(boot_image);
        fclose(image);
        exit(1);
    }

    /* put the second sector of stage1 bootsector */
    if (!write_filesec(image, 1, 1, readbuff)) {
        printf("error writing second sector of image.\n");
        fclose(boot_image);
        fclose(image);
        exit(1);
    }

    /* open stage1 image */
    memset(buffer, 0, 256);
    sprintf(buffer, "%sstage1.IMG", prefix);
    boot_image = fopen(buffer, "rb");
    if (!boot_image) {
        printf("error opening the CHB stage1 image. %s.\n", buffer);
        fclose(boot_image);
        fclose(image);
        exit(1);
    }

    /* determinate the file size in sectors */
    int stage1_size = determinate_file_len(boot_image);

    /*
     * the stage1 can't have It cannot be larger than the number of
     * sectors reserved in the FAT file system, in addition to this CHB
     * occupies the first 2 sectors in the boot program. so 2 must be
     * subtracted at the end of the check.
    */
    if (stage1_size > image_bootsec.reserved_sectors - 2) {
        printf("internal error: the stage1 is too big.\n");
        exit(1);
    }

    /* copy stage1 to image */
    for (int i = 0; i < stage1_size; i++) {
        /* read stage1 */
        char stage1buff[512];
        read_filesec(boot_image, i, 1, stage1buff);
        write_filesec(image, i + 2, 1, stage1buff);
    }

    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);

    fclose(image);
    fclose(boot_image);
}

int main(int argc, char** argv) {

    int optcount;
    int opt_index = 1;
	
	const char* img;
	const char* image_file;
	bool print_sec = false;
	bool make_chb = false;
	
    while (1) {
        optcount = getopt_long (argc, argv, "b:p:m:uh", long_options, &opt_index);
        if (optcount == -1) {
            break; /* end of the options */
        }

        switch (optcount) {
            case 'b': {
                img = optarg;
				print_sec = true;
                break;
            }
            case 'u': {
                usage(PROGRAM_NAME, 0);
                break;
            }
            case 'h': {
                usage(PROGRAM_NAME, 0);
                break;
            }
            case 'p': {
                prefix = optarg;
                break;
            }
            case 'm': {
				make_chb = true;
                image_file = optarg;
                break;
            }
            default: {
                usage(PROGRAM_NAME, 1);
                break;
            }
        }
    }
	
	if (make_chb)
		make_chb_image(image_file);
	
	if (print_sec)
		print_bootsector(img);
	
	exit(0);
}