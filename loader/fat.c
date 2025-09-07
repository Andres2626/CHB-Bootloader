
/*
* fat.c -- simple FAT FS controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/fs/fat.h>

/* memory definitions  */
#include <CHB/mm.h>

#include <CHB/ctype.h>
#include <CHB/disk.h>
#include <CHB/errno.h>
#include <CHB/minmax.h>
#include <CHB/stdio.h>
#include <CHB/string.h>

#define SAVE_ERR_RET(err, ret) set_errno(err); \
        return ret

/* FAT filesystem misc structures */

struct FAT_bootsec {
   uint8_t jmp[3];
   uint8_t oem[8];
   uint16_t sector_size;
   uint8_t cluster_size;
   uint16_t reserved_sectors;
   uint8_t FAT_count;
   uint16_t dir_entries;
   uint16_t drive_size;
   uint8_t desc_type;
   uint16_t FAT_size;
   uint16_t track_size;
   uint16_t drive_heads;
   uint32_t hidden_sectors;
   uint32_t large_sectors;

   /* EBR */
   uint8_t drive_number;
   uint8_t reserved;
   uint8_t signature;
   uint32_t volumeid;
   uint8_t label[11];
   uint8_t id[8];
} __attribute__((packed));

struct FAT_entry {
   uint8_t name[11];
   uint8_t attributes;
   uint8_t _Reserved;
   uint8_t created_time_tenths;
   uint16_t created_time;
   uint16_t created_date;
   uint16_t accessed_date;
   uint16_t first_clusterhi;
   uint16_t modified_time;
   uint16_t modified_date;
   uint16_t first_clusterlo;
   uint32_t size;
};

struct FAT_data {
   union {
      struct FAT_bootsec bs;
      uint8_t bytes[SEC_SIZE];
   } BS;

   struct FAT_file rootdir;

   struct FAT_file opened[MAX_FILE_HANDLES];
};

static struct FAT_data* f_data;
static uint8_t* fat;
static uint32_t datalba;

static int
fat_readbs(struct device* dev) {
   return devread(dev, 0, 1, f_data->BS.bytes);
}

static int
fat_readfat(struct device* dev) {
   return devread(dev, f_data->BS.bs.reserved_sectors, f_data->BS.bs.FAT_size, fat);
}

int
fat_init(struct device* dev) {
   f_data = (struct FAT_data*)FS_DRIVER_DATA;
   uint32_t fat_size;
   uint32_t rootdir, rootsize, data;
   
   /* Read Filesystem bootsector */
   if (!fat_readbs(dev)) {
	  printf("FAT_driver(): Error reading bootsector.");
      SAVE_ERR_RET(ERR_DISK_READ, 0);
   }

   fat = (uint8_t*)f_data + sizeof(struct FAT_data);
   fat_size = f_data->BS.bs.sector_size * f_data->BS.bs.FAT_size;
   if (sizeof(struct FAT_data) + fat_size > FS_DRIVER_DATA_SIZE) {
      SAVE_ERR_RET(ERR_NO_MEM, 0);
   }

   if (!fat_readfat(dev)) {
	  printf("FAT_driver(): Error reading bootsector.");
      SAVE_ERR_RET(ERR_DISK_READ, 0);
   }

   rootdir = f_data->BS.bs.reserved_sectors + (f_data->BS.bs.FAT_size * f_data->BS.bs.FAT_count);
   rootsize = sizeof(struct FAT_entry) * f_data->BS.bs.dir_entries;

   f_data->rootdir.handle = ROOT_DIRECTORY_HANDLE;
   f_data->rootdir.directory = true;
   f_data->rootdir.pos = 0;
   f_data->rootdir.size = rootsize;
   f_data->rootdir.opened = true;
   f_data->rootdir.first_cluster = rootdir;
   f_data->rootdir.current_cluster = rootdir;
   f_data->rootdir.current_sector = 0;

   /* read root directory */
   if (!devread(dev, rootdir, 1, f_data->rootdir.buffer)) {
      printf("FAT_driver(): Error reading root directory.");
      SAVE_ERR_RET(ERR_DISK_READ, 0);
   }

   data = (rootsize + f_data->BS.bs.sector_size - 1) / f_data->BS.bs.sector_size;
   datalba = rootdir + data;

   for (int i = 0; i < MAX_FILE_HANDLES; i++) {
      f_data->opened[i].opened = false;
   }

   SAVE_ERR_RET(ERR_NO, 1);
}

uint32_t
fat_cluster2lba(uint32_t cluster) {
   return datalba + (cluster - 2) * f_data->BS.bs.cluster_size;
}

struct FAT_file*
fat_open_entry(struct device* dev, struct FAT_entry* entry) {
   int handle = -1;
   for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++) {
      if (!f_data->opened[i].opened) {
         handle = i;
      }
   }

   if (handle < 0) {
      SAVE_ERR_RET(ERR_TOO_FILES, NULL);
   }

   struct FAT_file* fd = &f_data->opened[handle];
   fd->handle = handle;
   fd->directory = (entry->attributes & FAT_DIRECTORY) != 0;
   fd->pos = 0;
   fd->size = entry->size;
   fd->first_cluster = entry->first_clusterlo + ((uint32_t)entry->first_clusterhi << 16);
   fd->current_cluster = fd->first_cluster;
   fd->current_sector = 0;

   if (!devread(dev, fat_cluster2lba(fd->current_cluster), 1, fd->buffer)) {
      printf("FAT_driver(): Entry open failed.\n");
      SAVE_ERR_RET(ERR_DISK_READ, 0);
   }

   fd->opened = true;
   return fd;
}

uint32_t
fat_next_clutser(uint32_t current_cluster) {
   uint32_t index = current_cluster * 3 / 2;

   if (current_cluster % 2 == 0) {
      return (*(uint16_t*)(fat + index)) & 0x0FFF;
   } else {
      return (*(uint16_t*)(fat + index)) >> 4;
   }
}

uint32_t
fat_read(struct device* dev, struct FAT_file* file, uint32_t count, void* buff) {
   struct FAT_file* fd = (file->handle == ROOT_DIRECTORY_HANDLE) ? &f_data->rootdir : &f_data->opened[file->handle];
   uint8_t* dataout = buff;
   uint16_t left, take;

   if (!fd->directory || (fd->directory && fd->size != 0)) {
      count = min(count, fd->size - fd->pos);
   }

   while (count > 0) {
      left = SEC_SIZE - (fd->pos % SEC_SIZE);
      take = min(count, left);

      memcpy(dataout, fd->buffer + fd->pos % SEC_SIZE, take);
      dataout += take;
      fd->pos += take;
      count -= take;

      if (left == take) {
         if (fd->handle == ROOT_DIRECTORY_HANDLE) {
            ++fd->current_cluster;
            if (!devread(dev, fd->current_cluster, 1, fd->buffer)) {
               set_errno(ERR_DISK_READ);
               break;
            } else {
               if (++fd->current_sector >= f_data->BS.bs.cluster_size) {
                  fd->current_sector = 0;
                  fd->current_cluster = fat_next_clutser(fd->current_cluster);
               }

               /* check EOF */
               if (fd->current_cluster == FAT12_EOF) {
                  fd->size = fd->pos;
                  break;
               }

               /* read next sector */
               if (!devread(dev, fat_cluster2lba(fd->current_cluster) + fd->current_sector, 1, fd->buffer)) {
                  set_errno(ERR_DISK_READ);
                  break;
               }
            }
         }
      }
   }

   set_errno(ERR_NO);
   return dataout - (uint8_t*)buff;
}

bool
fat_read_entry(struct device* dev, struct FAT_file* file, struct FAT_entry* entry) {
   return fat_read(dev, file, sizeof(struct FAT_entry), entry) == sizeof(struct FAT_entry);
}

void
fat_close(struct FAT_file* file) {
   if (file->handle == ROOT_DIRECTORY_HANDLE) {
      file->pos = 0;
      f_data->rootdir.current_cluster = f_data->rootdir.first_cluster;
   } else {
      f_data->opened[file->handle].opened = false;
   }
}

bool
fat_find_file(struct device* dev, struct FAT_file* file, const char* name, struct FAT_entry* entry) {
   char fatname[12];
   struct FAT_entry en;
   memset(fatname, ' ', sizeof(fatname));
   fatname[11] = '\0';

   const char* ext = strchr(name, '.');
   if (ext == NULL) {
      ext = name + 11;
   }

   for (int i = 0; i < 8 && name[i] && name + i < ext; i++) {
      fatname[i] = toupper(name[i]);
   }

   if (ext != name + 11) {
      for (int i = 0; i < 3 && ext[i + 1]; i++) {
         fatname[8 + i] = toupper(ext[i + 1]);
      }
   }

   while (fat_read_entry(dev, file, &en)) {
      if (memcmp(fatname, en.name, 11) == 0) {
         *entry = en;
         return true;
      }
   }

   return false;
}

struct FAT_file*
fat_open(struct device* dev, const char* path) {
   char name[MAX_PATH_SIZE];
   struct FAT_entry entry;
   bool last;
   const char* delim;
   size_t len;
   struct FAT_file* current = &f_data->rootdir;

   if (path[0] == '/') {
      path++;
   }

   while (*path) {
      last = false;
      delim = strchr(path, '/');
      if (delim) {
         memcpy(name, path, delim - path);
         name[delim - path + 1] = '\0';
         path = delim + 1;
      } else {
         len = strlen(path);
         memcpy(name, path, len);
         name[len + 1] = '\0';
         path += len;
         last = true;
      }

      if (fat_find_file(dev, current, name, &entry)) {
         fat_close(current);
         if (!last && (entry.attributes & FAT_DIRECTORY) == 0) {
            SAVE_ERR_RET(ERR_FSYS_ATTR, NULL);
         }
         current = fat_open_entry(dev, &entry);
      } else {
		 /* file is not found */
         fat_close(current);
         return NULL;
      }
   }

   return current;
}
