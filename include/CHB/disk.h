
/*
 * disk.h -- disk and data access
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _DISK_H_
#define _DISK_H_ 1

#include <CHB/types.h>

#define ASM           __attribute__((cdecl))

#define DRIVE_STATE_1 1 /* no errors */

struct device {
   uint8_t number;
   uint16_t cyl;
   uint16_t sec;
   uint16_t head;
   int state; /* initialized */
};

/* int13_handler functions */
extern int ASM disk_reset_controller(uint8_t drive);
extern int ASM disk_get_parameters(uint8_t drive, uint8_t* type, uint16_t* cylinders, uint16_t* sectors,
                                   uint16_t* heads);
extern int ASM disk_read(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, void* out);

/* loader/disk.c ======== */

/* read disk sectors */
int devread(struct device* dev, int sector, int count, uint8_t* buff);

/* initialize disk system */
int devinit(struct device* dev);

#endif /* !_DISK_H_ */