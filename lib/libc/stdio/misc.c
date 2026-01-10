
#include "sys\stdio.h"
#include "shared\shared.h" /* VGA_WIDTH */

uint8_t console_color;
uint16_t console_index;

void set_color(uint8_t color) {
	console_color = color;
}

uint8_t get_color() {
	return console_color;
}

void gotoindex(uint16_t index) {
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(index & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((index >> 8) & 0xFF));

	console_index = index;
}

void gotoxy(uint8_t x, uint8_t y) {
	gotoindex(y * VGA_WIDTH + x);
}

uint16_t get_index() {
	return console_index;
}