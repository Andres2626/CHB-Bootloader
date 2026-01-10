
#include "sys\stdio.h" /* puthar(char) */
#include "shared\shared.h" /* VGA_MEMORY */

#define _PRINT_CHARACTER(print, index) \
	*(VGA_MEMORY + index * 2) = print; \
	*(VGA_MEMORY + index * 2 + 1) = get_color();

void putchar(char ch) {
	uint16_t index = get_index();

	switch (ch) {
	case '\0':
		_PRINT_CHARACTER(0x00, index);
		index++;
		break;
	case '\'':
		_PRINT_CHARACTER(0x27, index);
		index++;
		break;
	case '\"':
		_PRINT_CHARACTER(0x22, index);
		index++;
		break;
	case '\?':
		_PRINT_CHARACTER(0x3f, index);
		index++;
		break;
	case '\\':
		_PRINT_CHARACTER(0x5c, index);
		index++;
		break;
	case '\b':
		_PRINT_CHARACTER(0x00, index);
		index--;
		break;
	case '\f':
		index += VGA_WIDTH;
		break;
	case '\t':
		index = (index + 8) & ~(8 - 1);
		break;
	case '\v':
		index += VGA_WIDTH;
		break;
	case 10:
		index -= index % VGA_WIDTH - VGA_WIDTH;
		break;
	case 13:
		index -= index % VGA_WIDTH;
		break;
	default:
		{
			_PRINT_CHARACTER(ch, index);
			index++;
			break;
		}
	}

	gotoindex(index);
}