
#ifndef _DISK_H_
#define _DISK_H_ 1

#include <CHB/types.h>

int drive_reset_controller(uint8_t drive);
int drive_get_parameters(uint8_t drive, uint8_t* type, uint16_t* cylinders, uint16_t* sectors, uint16_t* heads);
int drive_read(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, void* out);

#endif /* !_DISK_H_ */