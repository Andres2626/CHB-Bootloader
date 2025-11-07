
/*
* putchar.c -- CHB putchar function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/stdio.h"

#include <sys/video.h>

PRIVATE u8t _colors[DARK_GRAY] = {
	0, 1, 2, 3, 4, 5, 6, 7
};

PRIVATE u8t ansi_to_vga_fg(int code) 
{
	if (code < 30 || code > 37) 
		return LIGHT_GRAY;
	
	return _colors[code - 30];
}

PRIVATE u8t ansi_to_vga_bg(int code)
{
	if (code < 40 || code > 47) 
		return BLACK;
	
	return _colors[code - 40];
}

void putchar(int c) 
{
	/* get colors from VGA driver */
    
    struct video *video_info = video_driver_get_info();
	u8t fg = video_info->fg;
	u8t bg = video_info->bg;
    int in_escape = 0, in_csi = 0, code = 0;
	
	if (!in_escape && c == '\033') { 
        in_escape = 1; 
        return; 
    }

    if (in_escape && !in_csi) {
        if (c == '[') 
			in_csi = 1;
        else { 
		    in_escape = 0; 
			in_csi = 0; 
		}
        return;
    }

    if (in_escape) {
        if (c >= '0' && c <= '9') { 
		    code = code * 10 + (c - '0'); 
			return; 
		}
        if (c == 'm') {
            if (code == 0) { 
			    fg = LIGHT_GRAY; 
				bg = BLACK; 
			}
            else if (code >= 30 && code <= 37) 
				fg = ansi_to_vga_fg(code);
            else if (code >= 40 && code <= 47) 
				bg = ansi_to_vga_bg(code);
			
            in_escape = 0; 
			in_csi = 0; 
			code = 0; 
			return;
        }
		
        in_escape= 0; 
		in_csi = 0; 
		code = 0;
        return;
    }
	
    video_driver_putchar(bg, fg, c);
}