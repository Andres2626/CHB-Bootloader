
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

PRIVATE void lba2chs(struct device *dev, int sector, u16t *cyl, u16t *sec, u16t *head) 
{
    struct device_geometry *g = &dev->geom;
    
    *sec = sector % g->sector + 1;
    *cyl = (sector / g->sector) / g->head;
    *head = (sector / g->sector) % g->head;
}

int devread(struct device *dev, u64t sector, u16t count, void *buff) 
{   
    /* check if drive is not initialized */
    if (dev->state != DRIVE_OK)
        return SIGN(EIO);
    
    /* 
     * The limit for reading sectors adjusted for both CHS and EDD is 128 sectors per operation 
     * due to memory limitations.
     */
    if (count == 0 || count > 0x7f)
        return SIGN(EINVAL);
   
    /* 
     * check if sector is not 'out-of-bounds' 
     */
    if (sector >= dev->geom.total_sectors)
        return SIGN(EINVAL);
    
    /* check if sector size is valid */
    if (dev->geom.sector_size % 512 != 0)
        return SIGN(EINVAL);
    
    if (dev->ext) {
        struct device_edd_packet edd;
        u32t lin = (u32t)buff;
        
        edd.size = 0x10;
        edd.reserved = 0;
        edd.count = count;
        edd.offset = lin & 0xf;
        edd.segment = lin >> 4;
        edd.lba = sector;
        
        /* read in extended mode */
        for (int i = 0; i < 3; i++) {    
            if (disk_edd_read(dev->number, &edd)) 
                goto out;     
            disk_reset_controller(dev->number);
        }
        goto fail;     
    }
    else {
        /* read in CHS mode */
        
        /* convert absolute LBA sector to CHS */
        u16t cyl, sec, head;  
        lba2chs(dev, (int)sector, &cyl, &sec, &head);
       
        for (int i = 0; i < 3; i++) {   
            if (disk_read(dev->number, cyl, sec, head, (u8t)count, buff)) 
                goto out;
            disk_reset_controller(dev->number);
        }

        goto fail;   
    }
    
fail:
    return SIGN(EIO);
out:
    return count * dev->geom.sector_size; /* return read bytes */
}

int devinit(struct device *dev) 
{
    struct device_geometry g;
    
    /* check BIOS HDD bit and extensions */
    bool chk = (dev->number & 0x80) && disk_get_extensions(dev->number);
    if (!chk)
        goto chs_mode;
    
    /* get extended parameters */
    g.size = 0x1e; /* for 2.0 edd */
    if (!disk_get_edd_parameters(dev->number, &g))
        goto chs_mode;
    
    /* check if sector size is valid */
    if (g.sector_size % 512 != 0)
        goto chs_mode;
    
    goto out;
    
chs_mode:
    u16t cyl, sec, head;
    
    /* get CHS parameters */
    if (!disk_get_parameters(dev->number, 0, &cyl, &sec, &head))
        return SIGN(EIO);
    
    /* push all parameters */
    g.cyl = (u32t)cyl;
    g.sector = (u32t)sec;
    g.head = (u32t)head;
    g.total_sectors = (u64t)cyl * (u64t)sec * (u64t)head;
    g.sector_size = 512;   
    
out:
    printf("disk detected number: 0x%x, type: %s\n", dev->number, chk ? "LBA" : "CHS");
    
    /* push disk info' */
    dev->ext = chk;
    dev->geom = g;
    dev->state = DRIVE_OK;
    return 1;
}