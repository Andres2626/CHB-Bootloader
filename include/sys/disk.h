
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

#define DRIVE_STATE_1 1 /* no errors */

struct device_geometry {
    u16t cyl;
    u16t head;
    u16t sector;
};

struct device {
   u8t number; /* reported by BIOS */
   int state; /* initialized */
   struct device_geometry geom;
};

/* int13_handler functions */
PUBLIC int _ASM disk_reset_controller(u8t drive);
PUBLIC int _ASM disk_get_parameters(u8t drive, u8t* type, u16t* cylinders, u16t* sectors,
                                   u16t* heads);
PUBLIC int _ASM disk_read(u8t drive, u16t cylinder, u16t sector, u16t head, u8t count, void* out);

/* loader/disk.c ======== */

/* read disk sectors */
PUBLIC int devread(struct device* dev, int sector, int count, u8t* buff);

/* initialize disk system */
PUBLIC int devinit(struct device* dev);

#endif /* !_DISK_H_ */