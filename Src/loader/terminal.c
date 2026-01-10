
/* entry.c -- C part of the loader */

#include <CHB/types.h>
#include <CHB/video.h>
#include <CHB/port.h>

#include <CHB/stdio.h>
#include <CHB/string.h>

char**
terminal_parse_argv(char* cmd) {
	char **argv;
	int count = 0;
	
	/* separate input considering spaces */
	char *token = strtok(cmd, " ");
	while (token != NULL) {
		argv[count] = token;
		token = strtok(NULL, " ");
		count++;
	}
	return argv;
}

uint8_t
terminal_parse_argc(char* cmd) {
	uint8_t count = 0;
	
	/* check how many spaces there are */
	while((cmd = strchr(cmd, 0x20)) != NULL) {                                                                                                                       
		count++;                                                                                                             
		cmd++;
	}
	return count;
}

void 
terminal_execute_command(int argc, char **argv) {
	if (argv[0][0] == '\0')
		return; /* NULL command */
	
	if (!strncmp(argv[0], "help\0", 5))
		command_help(argc, argv);
	else if (!strncmp(argv[0], "driveinfo\0", 10))
		command_driveinfo(argc, argv);
	else if (!strncmp(argv[0], "clear\0", 6))
		command_clear(argc, argv);
	else
		printf("%s command not found. Type help for more info.\n", argv[0]);
}