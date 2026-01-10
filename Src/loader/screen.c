
/* entry.c -- C part of the loader */

#include <CHB/types.h>
#include <CHB/video.h>
#include <CHB/port.h>

uint8_t video_mode = 0;
uint8_t video_cursor_start = 0, video_cursor_end = 0;
uint16_t video_color = 0;
uint16_t video_cursor_index = 0;
uint8_t *video_mm;

void
video_initialize(uint8_t mode) {
	/* set BIOS video mode */
	video_mode = video_set_mode(mode);
	
	/* setup cursor */
	video_cursor_enable(14, 15);
	
	/* set video MM loc */
	video_mm = VIDEO_TEXT_MEMORY;
	
	/* background: black and foreground: gray */
	video_cursor_set_color(0x0, 0x7);
	
	/* clean display */
	video_cls();
}

void 
video_cursor_enable(uint8_t start, uint8_t end) {	
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);
	port_outb(0x3D4, 0x0B);
	port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
	
	video_cursor_start = start;
	video_cursor_end = end;
}

void 
video_cursor_disable(void) {
	port_outb(0x3D4, 0x0A);
	port_outb(0x3D4, 0x20);
}

void 
video_cursor_gotoindex(uint16_t index) {
	port_outb(0x3D4, 0x0F);
	port_outb(0x3D5, (uint8_t) (index & 0xFF));
	port_outb(0x3D4, 0x0E);
	port_outb(0x3D5, (uint8_t) ((index >> 8) & 0xFF));
	video_cursor_index = index;
}

void 
video_cursor_gotoxy(uint8_t x, uint8_t y) {
	video_cursor_gotoindex(y * 80 + x);
}

uint16_t 
video_cursor_getindex(void) {
	return video_cursor_index;
}

void 
video_putchar(char c) {
	uint16_t index = video_cursor_getindex();
	
	/* backspace */
	if (c == '\b') {
		index--;
		*(video_mm + index * 2) = 0x0;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
	}
	/* vertical tab */
	else if (c == '\f' || c == '\v') {
		index += 80;
	}
	/* horizontal tab */
	else if (c =='\t') {
		index = (index + 8) & ~(8 - 1);
	}
	/* newline */
	else if (c == '\n') {
		index -= index % 80 - 80;
	}
	/* carriage return */
	else if (c == '\r') {
		index -= index % 80;
	}
	/* any printable character */
	else if (c >= 0x20) {
		*(video_mm + index * 2) = c;
		*(video_mm + index * 2 + 1) = video_cursor_get_color();
		index++;
	}

	video_cursor_gotoindex(index);
	video_scroll();
}

void 
video_cursor_set_color(uint8_t background, uint8_t foreground) {
	video_color = (background << 4) | (foreground & 0x0F);
}

uint16_t 
video_cursor_get_color(void) {
	return video_color;
}

void 
video_cls(void) {
	video_cursor_gotoxy(0, 0);
	
	for (int i = 0; i < (80 * 25); i++) {
		video_putchar(' ');
	}
	
	video_cursor_gotoxy(0, 0);
}

void 
video_scroll(void) {
	/* convert index to x, y coordinates */
	uint8_t cursor_x = video_cursor_getindex() % 80;
	uint8_t cursor_y = (video_cursor_getindex() - cursor_x) / 80;
	uint16_t* loc = (uint16_t*)0xb8000;
	video_cursor_set_color(0x0, 0x7);
	
	uint8_t attributeByte = video_cursor_get_color();
    uint16_t blank = 0x20 /* space */ | (attributeByte << 8);
	
	if (cursor_y >= 25) {
        for (int i = 0; i < 24*80; i++) {
            loc[i] = loc[i+80];
        }

        for (int i = 24*80; i < 25*80; i++) {
            loc[i] = blank;
        }

        cursor_y = 24;
	}

	video_cursor_gotoxy(cursor_x, cursor_y);
}