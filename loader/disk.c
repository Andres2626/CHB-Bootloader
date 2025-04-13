
/*
* disk.c -- simple disk controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/disk.h>
#include <CHB/errno.h>

#include <CHB/stdio.h>

static void
lba2chs(struct device* dev, int sector, uint16_t* cyl, uint16_t* sec, uint16_t* head) {
   *sec = sector % dev->sec + 1;
   *cyl = (sector / dev->sec) / dev->head;
   *head = (sector / dev->sec) % dev->head;
}

int
devread(struct device* dev, int sector, int count, uint8_t* buff) {
   uint16_t cyl = 0, sec = 0, head = 0;
   if (dev->state != 1) {
      return EDEV; /* device not initialized */
   }
   if (sector < 0 || (count < 0 || count > 0x80)) {
      return ESEC; /* invalid sector */
   }
   lba2chs(dev, sector, &cyl, &sec, &head);
   for (int i = 0; i < 3; i++) {
      if (disk_read(dev->number, cyl, sec, head, count, buff)) {
         return ENO; /* no error */
      }
      disk_reset_controller(dev->number);
   }

   return EREAD; /* device read error. */
}

int
devinit(struct device* dev) {
   if (!disk_reset_controller(dev->number)) {
      return EDEV;
   }
   if (!disk_get_parameters(dev->number, 0, &dev->cyl, &dev->sec, &dev->head)) {
      return EDEV;
   }
   dev->state = DRIVE_STATE_1;
   return ENO;
}