
/*
* video.c -- VGA video simple controller
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <sys/video.h>
#include <sys/port.h>

#include "types.h"
#include "lib/libc/stdio.h"

PRIVATE struct video *video_info;
PRIVATE u16t *video_mm = (u16t*)VIDEO_DRIVER_MM;

void video_init(struct video *vid) 
{
    struct video_cursor csr = vid->cursor;
    
    video_driver_set_mode(vid->mode);
	video_driver_cls(vid->bg, vid->fg);	
	
	if (!vid->cursor.state)
		video_driver_cursor_disable();
	else
	{
		video_driver_cursor_enable(csr.start, csr.end);
		video_driver_cursor_gotoxy(csr.x, csr.y);
	}
    
    /* setup video info */
    video_info = vid;
    
    printf("[VIDEO] Initialized, mode: %i, cursor: %i, start: %i, end: %i, x: %i, y: %i, "
           "bg: %i, fg: %i\n",
	       vid->mode, csr.state, csr.start, csr.end, csr.x, csr.y, vid->bg, vid->fg);
}

void video_driver_cursor_enable(u8t start, u8t end) 
{
    port_outb(0x3D4, 0x0A);
    port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);
    port_outb(0x3D4, 0x0B);
    port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
    
    video_info->cursor.state = true;
    video_info->cursor.start = start;
    video_info->cursor.end = end;
}

void video_driver_cursor_disable(void) 
{
    port_outb(0x3D4, 0x0A);
    port_outb(0x3D5, 0x20);
    video_info->cursor.state = false;
}

void video_driver_cursor_gotoindex(u16t index) 
{
    port_outb(0x3D4, 0x0F);
    port_outb(0x3D5, (u8t)(index & 0xFF));
    port_outb(0x3D4, 0x0E);
    port_outb(0x3D5, (u8t)((index >> 8) & 0xFF));
    video_info->cursor.pos = index;
}

void video_driver_cursor_gotoxy(u8t x, u8t y) 
{
    video_driver_cursor_gotoindex(y * 80 + x);
    video_info->cursor.x = x;
    video_info->cursor.y = y;
}

void video_driver_putchar(u8t bg, u8t fg, char c) 
{
    u16t index = video_info->cursor.pos;
    u16t color = (bg << 4) | (fg & 0x0F);
    
    switch (c) {
        case '\b':
            /* avoid negative index */
            if (index > 0) {
                index--;
                video_mm[index] = (color << 8) | 0x20;
            }
            break;
        case '\v':
        case '\f': 
		    index += VIDEO_WIDTH; 
            break;
        case '\t': 
		    index = (index + 8) & ~(8 - 1); 
            break;
        case '\n': 
		    index += VIDEO_WIDTH - (index % VIDEO_WIDTH); 
            break;
        case '\r': 
		    index -= index % VIDEO_WIDTH; 
            break;
        default:
            video_mm[index] = (color << 8) | c;
            index++;
            break;
    }
    video_driver_cursor_gotoindex(index);
    video_driver_scroll(bg, fg);
}

void video_driver_cls(u8t bg, u8t fg) 
{
	u16t color = (bg << 4) | (fg & 0x0F);
    u16t blank = (color << 8) | ' ';
    
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++)
        video_mm[i] = blank;

    video_driver_cursor_gotoxy(0, 0);
}

void video_driver_scroll(u8t bg, u8t fg) 
{
    struct video_cursor csr;
    csr.x = video_info->cursor.pos % VIDEO_WIDTH;
    csr.y = (video_info->cursor.pos - csr.x) / VIDEO_WIDTH;
    
    u16t color = (bg << 4) | (fg & 0x0F);
    u16t *loc = (u16t*)VIDEO_DRIVER_MM;
    u8t attr = color;
    u16t blank = 0x20 | (attr << 8);
    
    if (csr.y >= VIDEO_HEIGHT) {
        for (int i = 0; i < 24 * VIDEO_WIDTH; i++)
            loc[i] = loc[i + VIDEO_WIDTH];
	 
        for (int i = (VIDEO_HEIGHT - 1) * VIDEO_WIDTH; i < VIDEO_HEIGHT * VIDEO_WIDTH; i++)
            loc[i] = blank;
	 
        csr.y = VIDEO_HEIGHT - 1;
    }
    video_driver_cursor_gotoxy(csr.x, csr.y);
}

struct video *video_driver_get_info(void)
{
    return video_info;
}