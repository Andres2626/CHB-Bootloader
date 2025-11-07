
#include <sys/mm.h>

#include "lib/libc/errno.h"
#include "lib/libc/stdio.h"

PRIVATE struct memory_block memory_regs[MEMORY_MAX_REGIONS];

int memory_init(struct memory_info *info) 
{
    struct memory_block block;
	int count = 0;
	u32t continuation = 0;
    
	printf("[MM] Checking memory\n");
	
	/* get upper and lower memory */
	info->lo = get_lower_memory();
	info->hi = get_upper_memory();
	
	if (info->lo <= 0) {
		printf("[MM] INT12, failed\n");
		set_errno(EMEMINIT);
		return 0;
	}
	
	if (info->hi == -1) {
		printf("[MM] INT15, %ah=0x88 failed\n");
		set_errno(EMEMINIT);
		return 0;
	}
	
	printf("[MM] lower: %iK, upper: %iK\n", info->lo, info->hi);
	
	/* get memory via INT15, E820 */
	int ret = get_memory_block(&block, &continuation);
	if (ret == -1) {
		printf("[MM] INT15, %ah=0xE820 failed\n");
		set_errno(EMEMINIT);
		return 0;
	}
	
	while (ret > 0 && continuation != 0) {
		memory_regs[count] = block;
        
		printf("[MM] block, begin: 0x%llx, len: 0x%llx, type: %li, flag: 0x%lx\n", 
		       block.begin, block.len, block.type, block.flag);
               
		ret = get_memory_block(&block, &continuation);
        count++;
	}
	
	info->count = count;
	info->blk = memory_regs;
	return 1;
}