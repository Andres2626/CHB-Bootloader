
#ifndef _TERMINAL_H_
#define _TERMINAL_H_ 1

#include <CHB/types.h>

char** terminal_parse_argv(char* cmd);
uint8_t terminal_parse_argc(char* cmd);
void terminal_execute_command(int argc, char** argv);

#endif /* !_TERMINAL_H_ */