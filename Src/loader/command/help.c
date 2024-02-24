
extern int printf(const char* fmt, ...);

int command_help(int argc, char **argv) {
	printf("kernel <sector> <address>  -- read disk and push kernel in memory\n");
	printf("boot -- boot kernel at physical address 0x1000000\n");
	printf("clear -- clear the screen\n");
	printf("help -- print this\n");
}