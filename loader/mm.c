
#include <sys/mm.h>

#include "lib/libc/errno.h"
#include "lib/libc/stdio.h"

PRIVATE struct memory_block *memory_regs = (struct memory_block*)MM_BUFFER;

int memory_init(struct memory_info *info) 
{
    struct memory_block block;
	int count = 0;
	u32t continuation = 0;
	
	/* get upper and lower memory */
	info->lo = get_lower_memory();
	info->hi = get_upper_memory();
	
	if (info->lo <= 0)
        return SIGN(ESYSINIT);
	
	if (info->hi <= 0)
        return SIGN(ESYSINIT);
	
	/* get memory via INT15, E820 */
	int ret = get_memory_block(&block, &continuation);
	if (ret < 0)
        return SIGN(ESYSINIT);
	
	while (ret > 0 && continuation != 0) {
		memory_regs[count] = block;
		ret = get_memory_block(&block, &continuation);
        count++;
	}
	
	info->count = count;
	info->blk = memory_regs;
	return ENO;
}