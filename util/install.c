/* install.c -- Install CHB into FAT12/16 or MBR image */

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <argp.h>

#include <config.h>
#include <loader.h>
#include <stage1/stage1.h>
#include <sys/disk.h>
#include <fs/fat.h>

#define PROG_NAME "install"
#define PROG_VER  "1.0"

const char* argp_program_version = "CHB install 1.0";

union fat_bsec {
    struct fat_boot_sector info;
    char data[512];
};

/* obtained FAT info */
static union fat_bsec fs;

static char *input = NULL;
static char *prefix = NULL;
static char *first_boot_option = NULL;
static bool force_lba = false;

static int parse_opt(int key, char *arg, __attribute__((unused)) struct argp_state *state) 
{
    switch (key) {
    case 'b': 
        first_boot_option = arg;
        break;
    case 'f': 
        force_lba = true; 
        break;
    case 'i': 
        input = arg; 
        break;
    case 'p': 
        prefix = arg; 
        break;
    }
    return 0;
}

struct argp_option options[] = {
    {"first-boot-option", 'b', "FILE", 0, "Set kernel path in FS default=kernel.elf" },
    {"force-lba", 'f', 0, 0, "Force LBA addressing mode (if the disk is HDD) default=0" },
    {"input", 'i', "FILE", 0, "Input image, must be formatted for FAT12/16" },
    {"prefix", 'p', "DIR", 0, "Directory where CHB was installed."},
    {0}
};
    
struct argp argp = {options, parse_opt, 0, "Install CHB into FAT12/16 or MBR image\n"
                   "Warning: This program works direcly on disk images"};

int write_sector(u32t lba, char *data, FILE *img)
{
    fseek(img, lba * 512, SEEK_SET);
    if (fwrite(data, 1, 512, img) != 0)
        return 0;
    
    return 1;
}

int read_disk(u32t lba, char *data, FILE *img) 
{
    fseek(img, lba * 512, SEEK_SET);
    if (fread(data, 1, 512, img) != 0)
        return 0;
    
    return 1;
}

int main(int argc, char **argv) 
{
    FILE *in, *stage1, *loader;
    char buff[255];
    char sector[512];
    char install_sector[512];
    bool is_mbr = false;
    argp_parse(&argp, argc, argv, 0, 0, 0);
    
    if (!prefix)
        printf("warn: CHB build directory is not defined. try with --prefix=[DIR] option\n");

    if (!input) {
        printf("Input file is not defined. try with --input-fat-img=[FILE] option\n");
        return 1;
    }
    
    /* open input image for read and write */
    in = fopen(input, "r+");
    if (!in) {
        printf("error opening input image. %s\n", input);
        return 1;
    }
    
    /* read bootsector */
    if (read_disk(0, fs.data, in) != 0) {
        printf("error obtaining bootsector.\n");
        return 1;
    }
    
    struct mbr_table *part = (struct mbr_table*)(fs.data  + MBR_START);
    int i = 0;
    
    if (part->magic != 0xAA55) {
        printf("invalid boot sector signature.\n");
        return 1;
    }        

    /* 
     * check if have a FAT32 partition
     * NOTE: The partition must be marked as 'activated' 
     */
    for (i = 0; i < 4; i++) {
        if ((part->entry[i].type == 0x0B || part->entry[i].type == 0x0C) && 
            (part->entry[i].flags & 0x80) != 0) {
            /* FAT32 partition */
            goto fat32;
        }
    }
    
    /* the drive is a floppy */
    goto fat_check_type;
    
fat32:
    is_mbr = true;
    
    printf("Using MBR mode \n");
    
    /* check if there is enough space for the loader after the MBR and before the first VBR */
    if ((part->entry[0].lba < 2048))
        goto err_loader_big;
    
    goto install_stage1;
    
fat_check_type:
    is_mbr = false;
    
    printf("Using floppy mode\n");
    
    if (fs.info.reserved_sectors < 45)
        goto err_loader_big;
    
    /* get data info */
    u32t len = (fs.info.dir_entries * 32 + fs.info.sector_size - 1)  / fs.info.sector_size;
    u32t lba = fs.info.reserved_sectors + (fs.info.FAT_count * fs.info.FAT_size) + len;
    u32t sectors = (fs.info.drive_size == 0) ? fs.info.large_sectors : fs.info.drive_size;
    u32t datalen = sectors - lba;
    u32t clusters = datalen / fs.info.cluster_size;
    
    /* probe fat type */
    if (fs.info.dir_entries == 0 && fs.info.FAT_size == 0 && fs.info.fat32_size != 0) {   
        printf("FAT32 supperfloppy is not supported, use MBR instead.\n");
        return 1;
    }
    else if (fs.info.sector_size == 0)
        return 1; /* TODO: exfat not implemented */

install_stage1:

    /* open stage1 image for read */
    snprintf(buff, sizeof(buff), "%s/stage1.img", prefix);
    stage1 = fopen(buff, "rb");
    if (!stage1) {
        printf("error opening CHB stage1 image. %s\n", buff);
        return 1;
    }
    
    /* read bootsector */
    if (read_disk(0, install_sector, stage1) != 0) {
        printf("error obtaining CHB bootsector.\n");
        return 1;
    }
    
    fclose(stage1);
    
    memcpy(sector, fs.data, 512);
    
    /* copy 'jmp BPB_END nop' */
    sector[0] = install_sector[0];
    sector[1] = install_sector[1];
    sector[2] = install_sector[2];
    
    for (int x = 3; x < 512; x++) {
        /* do no copy MBR if image is FAT32 */
        if (is_mbr && x >= MBR_START)
            continue;
        
        if (!is_mbr && x <= BOOT_BPB_END)
            continue;   
        
        if (force_lba && x == 0x4c) {
            sector[x] = 1;
            continue;
        }
        
        sector[x] = install_sector[x];        
    }
    
    /* write stage1 */
    if (write_sector(0, sector, in) != 0) {
        printf("error installing first sector.\n");
        return 1;
    }
    
    /* open loader image for read */
    snprintf(buff, sizeof(buff), "%s/loader.img", prefix);
    loader = fopen(buff, "rb");
    if (!loader) {
        printf("error opening CHB loader image. %s\n", buff);
        return 1;
    }

    /* get loader size */
    fseek(loader, 0, SEEK_END);
    u32t loader_size = ftell(loader);
    fseek(loader, 0, SEEK_SET);
    
    /* convert to size in sectors */
    int cursec = 0;
    loader_size = (loader_size + 511) >> 9;
    
    while (loader_size) {
        
        /* initialize sector array */
        memset(sector, 0, 512);
        
        if (read_disk(cursec, sector, loader) != 0) {
            printf("error obtaining loader %i sector.\n", cursec);
            return 1;
        }
        
        if (cursec == 0) {
            
            struct loader_hdr *hdr = (struct loader_hdr*)(sector  + 0x4);
            
            /* check magic number and size */
            if (hdr->magic != CHB_MAGIC || hdr->size != CHB_HDR_SIZE_V06) {
                printf("invalid loader header\n");
                return 1;
            }
            
            /* push kernel file path to hdr */
            if (first_boot_option)
                memcpy(hdr->first_boot, first_boot_option, 32);
        }

        if (write_sector(cursec + 1, sector, in) != 0) {
            printf("error writing loader %i sector.\n", cursec);
            return 0;
        }

        cursec++;
        loader_size--;
    }
    
    fclose(in);
    fclose(loader);
    
    printf("installation terminated.\n");
    
    return 1;

err_loader_big:
    printf("loader is to big.\n");
    return 0;
}