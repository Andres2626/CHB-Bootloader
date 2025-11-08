
/*
* disk.c -- simple disk controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <sys/disk.h>

#include "lib/libc/errno.h"
#include "lib/libc/stdio.h"

#define SAVE_ERR_RET(err, ret) set_errno(err); \
        return ret

PRIVATE void lba2chs(struct device *dev, int sector, u16t *cyl, u16t *sec, u16t *head) 
{
   *sec = sector % dev->sec + 1;
   *cyl = (sector / dev->sec) / dev->head;
   *head = (sector / dev->sec) % dev->head;
}

int devread(struct device *dev, int sector, int count, u8t *buff) 
{
    u16t cyl, sec, head;
	
    if (dev->state != 1) {
        SAVE_ERR_RET(EDEVINT, 0);
    }
   
    if (sector < 0 || (count < 0 || count > 0x80)) {
        SAVE_ERR_RET(ESINV, 0);
    }
   
    /* convert absolute LBA sector to CHS */
    lba2chs(dev, sector, &cyl, &sec, &head);
   
    for (int i = 0; i < 3; i++) {
        if (disk_read(dev->number, cyl, sec, head, count, buff)) 
		    return 1;
	 
        disk_reset_controller(dev->number);
    }

    SAVE_ERR_RET(EREAD, 0);
}

int devinit(struct device *dev) 
{
	printf("[DISK] Initializing number: 0x%x\n", dev->number);
	
    if (!disk_get_parameters(dev->number, 0, &dev->cyl, &dev->sec, &dev->head)) {
        SAVE_ERR_RET(EDISK, 0);
    }
  
    /* save disk number as valid state */
    dev->state = DRIVE_STATE_1;
    return 1;
}