
#include <CHB/stdio.h>
#include <CHB/loader/drive.h>
#include <CHB/string.h>

extern uint8_t drive;
extern uint8_t type;
extern uint16_t cylinders;
extern uint16_t sectors;
extern uint16_t heads;

uint8_t drive_number = 0;

void driveinfo_print(uint8_t type, uint16_t C, uint16_t S, uint16_t H);

int 
command_driveinfo(int argc, char **argv) {
	if (!strncmp(argv[1], "\0", 1)) {
		printf("drive number not specified. Using currenly drive number\n");
		drive_number = drive;
		driveinfo_print(type, cylinders, sectors, heads);
	}
	else {
		drive_number = atoi(argv[1]);
		uint8_t ty;
		uint16_t C, S, H;
		if (!drive_get_parameters(drive_number, &ty, &C, &S, &H)) {
			printf("CHB error: error to get drive parameters\n");
			return -1;
		}
		driveinfo_print(ty, C, S, H);
	}
	
	return 0;
}

void 
driveinfo_print(uint8_t type, uint16_t C, uint16_t S, uint16_t H) {
	printf("drive number: 0x%x\n", drive_number);
	printf("drive type: ");
	switch (type) {
	case 1:
		printf("0x%x - 360K floppy\n", type);
		break;
	case 2:
		printf("0x%x - 1.2M floppy\n", type);
		break;
	case 3:
		printf("0x%x - 1.44M floppy\n", type);
		break;
	case 4:
		printf("0x%x - 2.88 floppy (drive type shipped)\n", type);
		break;
	case 5:
		printf("0x%x - 2.88 floppy\n", type);
		break;
	default:
		printf("0x%x - non-floppy\n", type);
	}
	
	printf("C: %i H: %i S: %i\n", C, H, S);
}