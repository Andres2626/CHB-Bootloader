
/*
 * disk.h -- disk and data access
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _DISK_H_
#define _DISK_H_ 1

#include "types.h"
#include "const.h"

#define DRIVE_OK 1 /* no errors */

struct device_geometry {
    /* 1.0+ */
    u16t size;
    u16t flags;
    u32t cyl;
    u32t head;
    u32t sector;
    u64t total_sectors;
    u16t sector_size;
    
    /* 2.0+ */
    u8t unused[39];
} __attribute__ ((packed));

struct device_edd_packet {
    u8t size;
    u8t reserved;
    u16t count;
    u16t offset;
    u16t segment;
    u64t lba;
};

struct device {
   u8t number; /* reported by BIOS */
   int state; /* initialized */
   bool ext; /* INT13,4X support */
   struct device_geometry geom;
};

/* int13_handler functions */
PUBLIC int _ASM disk_reset_controller(u8t drive);
PUBLIC int _ASM disk_get_parameters(u8t drive, u8t* type, u16t* cylinders, u16t* sectors, u16t* heads);
PUBLIC int _ASM disk_read(u8t drive, u16t cylinder, u16t sector, u16t head, u8t count, void* buff);
PUBLIC int _ASM disk_get_extensions(u8t drive);
PUBLIC int _ASM disk_get_edd_parameters(u8t drive, struct device_geometry* edd);
PUBLIC int _ASM disk_edd_read(u8t drive, struct device_edd_packet* edd);

/* read disk sectors */
PUBLIC int devread(struct device *dev, u64t sector, u16t count, void *buff);

/* initialize disk system */
PUBLIC int devinit(struct device* dev);

#endif /* !_DISK_H_ */