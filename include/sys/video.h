
/*
 * video.h -- video driver prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _VIDEO_H_
#define _VIDEO_H_ 1

#include "types.h"
#include "const.h"

#define VIDEO_DRIVER_MM 0xb8000
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25

/* cursor descriptor */
struct video_cursor {
    bool state; /* (0 = disabled, 1 = enabled) */
    u8t start;
    u8t end;
    u8t x;
    u8t y;
    u16t pos; /* reserved */
};

/* video properties */
struct video {
    u8t mode; /* BIOS mode */
	u8t bg; /* initial background */
	u8t fg; /* initial foreground */
    struct video_cursor cursor; /* cursor descriptor */
};

/* int10_handler functions */
PUBLIC u8t video_driver_set_mode(u8t mode);

/* video functions */
PUBLIC void video_init(struct video *vid);

/* cursor functions */
PUBLIC void video_driver_cursor_enable(u8t start, u8t end);
PUBLIC void video_driver_cursor_disable(void);
PUBLIC void video_driver_cursor_gotoindex(u16t index);
PUBLIC void video_driver_cursor_gotoxy(u8t x, u8t y);
PUBLIC u16t video_driver_cursor_getindex(void);

/* video mm write */
PUBLIC void video_driver_putchar(u8t, u8t, char);

/* video misc functions */
PUBLIC void video_driver_cls(u8t, u8t);
PUBLIC void video_driver_scroll(u8t, u8t);
PUBLIC void video_driver_set_color(u8t background, u8t foreground);
PUBLIC struct video *video_driver_get_info(void);

enum video_color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

#endif /* !_VIDEO_H_ */