
#ifndef __DISKK_H__
#define __DISK_H__

#include "shared\shared.h"
#include "sys\stdbool.h"

bool disk_get_geometry(disk_geom_info_t* DISK, uint8_t drive_number);
bool disk_readsectors(disk_geom_info_t* DISK, uint32_t lba, uint16_t sectors, void* buffer);

#endif