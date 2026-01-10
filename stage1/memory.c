
#include "memory.h" 

/* Define the MAXIMUM limit of memory block structures */
#define MAX_MEM_BLOCKS 256

/* store the memory blocks */
memory_block_t mem_regs[MAX_MEM_BLOCKS];
int ret;
uint32_t continuation, count;

int get_memory_info(memory_info_t* info) {
	
	info->lower_size = get_lower_memory();
	info->upper_size = get_upper_memory();
	
	if (info->lower_size == -1 || info->upper_size == -1) {
		return 0; /* call failed */
	}

	/* get the upper memory map */
	memory_block_t regs;
	continuation = 0;
	count = 0;
	ret = get_memory_block(&regs, &continuation);
	while (ret > 0 && continuation != 0) {
		mem_regs[count] = regs;
		count++;
		ret = get_memory_block(&regs, &continuation);
	}

	info->block_count = count;
	info->blocks = mem_regs;
	return 1;
}