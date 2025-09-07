
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

#define SAVE_ERR_RET(err, ret) set_errno(err); \
        return ret

static void
lba2chs(struct device* dev, int sector, uint16_t* cyl, uint16_t* sec, uint16_t* head) {
   *sec = sector % dev->sec + 1;
   *cyl = (sector / dev->sec) / dev->head;
   *head = (sector / dev->sec) % dev->head;
}

int
devread(struct device* dev, int sector, int count, uint8_t* buff) {
   uint16_t cyl, sec, head;
   
   /* check if disk is initialized */
   if (dev->state != 1) {
      SAVE_ERR_RET(ERR_DEVICE_INIT, 0);
   }
   
   /* check if sector to read is valid */
   if (sector < 0 || (count < 0 || count > 0x80)) {
      SAVE_ERR_RET(ERR_SECTOR_INV, 0);
   }
   
   /* convert absolute sector to CHS */
   lba2chs(dev, sector, &cyl, &sec, &head);
   
   /* read disk */
   for (int i = 0; i < 3; i++) {
      if (disk_read(dev->number, cyl, sec, head, count, buff)) {
         SAVE_ERR_RET(ERR_NO, 1);
      }
      disk_reset_controller(dev->number);
   }

   SAVE_ERR_RET(ERR_DISK_READ, 0);
}

int
devinit(struct device* dev) {
   if (!disk_get_parameters(dev->number, 0, &dev->cyl, &dev->sec, &dev->head)) {
      SAVE_ERR_RET(ERR_DISK_ERR, 0);
   }
   dev->state = DRIVE_STATE_1;
   return 1;
}