
#include "shared\disk.h"

void disk_sector_to_chs(disk_geom_info_t* disk, uint32_t lba, uint16_t* C, uint16_t* S, uint16_t* H) {	
	*S = lba % disk->sectors + 1;
    *C = (lba / disk->sectors) / disk->heads;
    *H = (lba / disk->sectors) % disk->heads;
}

bool disk_get_geometry(disk_geom_info_t* DISK, uint8_t drive_number) {
	uint8_t type;
	uint16_t cylinders, sectors, heads;

	if (!disk_parameters(drive_number, &type, &heads, &sectors, &cylinders)) {
		return false; /* call failed  */
	}

	DISK->drive_number = drive_number;
	DISK->type = type;
	DISK->cylinders = cylinders;
	DISK->sectors = sectors;
	DISK->heads = heads;

	return true;
}

bool disk_readsectors(disk_geom_info_t* DISK, uint32_t lba, uint16_t sectors, void* buffer) {
	uint16_t C, S, H;
	disk_sector_to_chs(DISK, lba, &C, &S, &H);

	printf("%i\n", (C * DISK->heads + H) * DISK->sectors + (S - 1)); 

	for (int i = 0; i < 3; i++) {
		if (disk_read(DISK->drive_number, C, S, H, sectors, buffer)) {
			return true; /* call failed  */
		}

		/* reset disk and retry read */
		disk_reset(DISK->drive_number);
	}

	return false;
}