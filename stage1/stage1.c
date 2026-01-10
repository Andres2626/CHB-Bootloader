
#include "stage1_functions.h"
#include "shared\shared.h"
#include "memory.h"
#include "shared\disk.h"
#include "sys\stdio.h"
#include "sys\stdlib.h"
#include "fsys.h"

uint8_t* buff = (void*)0x30000;

void 
entry(uint16_t drive_number) {
	/* load the stage1 Global Descriptor Table */
	load_new_gdt();
	
	/* set the video mode in 80x25 16 colors */
	set_video_mode(0x03);
	
	/* set color of terminal */
	set_color(BACK_BLACK | FORE_LIGHTGRAY);
	
	/* init disk */
	disk_geom_info_t disk;
	if (!disk_get_geometry(&disk, drive_number))  {
		printf("Error to initialize disk!\n");
		simple_exit(BAD_EXIT);
	}
	printf("Hello from stage1\n");
	
	uint16_t C, S, H;
	for (int i = 0; i < 10; i++)
	{
		disk_sector_to_chs(&disk, 35 + i, &C, &S, &H);
		printf("C: %i ", C);
		printf("S: %i ", S);
		printf("H: %i\n ", H);
		
	}

	disk_readsectors(&disk, 36, 1, buff);
	
	for (int i = 0; i < 512; i++) 
		printf("%x ", buff[i]);
}
