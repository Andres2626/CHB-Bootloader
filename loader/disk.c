
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
    struct device_geometry *g = &dev->geom;
    
    *sec = sector % g->sector + 1;
    *cyl = (sector / g->sector) / g->head;
    *head = (sector / g->sector) % g->head;
}

int devread(struct device *dev, int sector, int count, u8t *buff) 
{
    struct device_geometry g;
    
    /* check if drive is not initialized */
    if (dev->state != 1)
        return SIGN(EIO);
   
    /* 
     * check if sector is not 'out-of-bounds' 
     * TODO: This is not aplicable for CD or drive with INT13,4X
     */
    if (sector < 0 || (count < 0 || count > 0x80))
        return SIGN(EINVAL);
   
    /* convert absolute LBA sector to CHS */
    lba2chs(dev, sector, &g.cyl, &g.sector, &g.head);
   
    for (int i = 0; i < 3; i++) {
        if (disk_read(dev->number, g.cyl, g.sector, g.head, count, buff)) 
		    return count * 512; /* return read bytes */
	 
        disk_reset_controller(dev->number);
    }

    return SIGN(EIO);
}

int devinit(struct device *dev) 
{
    struct device_geometry g;
    
	printf("[DISK] Initializing number: 0x%x\n", dev->number);
	
    if (!disk_get_parameters(dev->number, 0, &g.cyl, &g.sector, &g.head))
        return SIGN(EIO);
  
    /* save disk as 'initialized' */
    dev->geom = g;
    dev->state = DRIVE_STATE_1;
    return 0;
}