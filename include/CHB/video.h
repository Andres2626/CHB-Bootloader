
/*
 * video.h -- video driver prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _VIDEO_H_
#define _VIDEO_H_ 1

#include <CHB/types.h>

#define VIDEO_DRIVER_MM (uint8_t*)0xb8000

/* int10_handler functions */
uint8_t video_driver_set_mode(uint8_t mode);

/* video functions */
void video_driver_init(uint8_t mode);

/* cursor functions */
void video_driver_cursor_enable(uint8_t start, uint8_t end);
void video_driver_cursor_disable(void);
void video_driver_cursor_gotoindex(uint16_t index);
void video_driver_cursor_gotoxy(uint8_t x, uint8_t y);
uint16_t video_driver_cursor_getindex(void);

/* video mm write */
void video_driver_putchar(uint8_t, uint8_t, char);

/* video misc functions */
void video_driver_cls(uint8_t, uint8_t);
void video_driver_scroll(uint8_t, uint8_t);
void video_driver_set_color(uint8_t background, uint8_t foreground);
uint16_t video_driver_get_color(void);

#endif /* !_VIDEO_H_ */