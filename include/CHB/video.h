
#ifndef _VIDEO_H_
#define _VIDEO_H_ 1

#include <CHB/types.h>

#define VIDEO_TEXT_MEMORY (uint8_t*)0xb8000

uint8_t video_set_mode(uint8_t mode); /* loader/video.S */

/* video functions */
void video_initialize(uint8_t mode);

/* cursor functions */
void video_cursor_enable(uint8_t start, uint8_t end);
void video_cursor_disable(void);
void video_cursor_gotoindex(uint16_t index);
void video_cursor_gotoxy(uint8_t x, uint8_t y);
uint16_t video_cursor_getindex(void);
void video_cursor_set_color(uint8_t background, uint8_t foreground); /* implement graphic modes */
uint16_t video_cursor_get_color(void);

/* video mm write */
void video_putchar(char c);

/* video misc functions */
void video_cls(void);
void video_scroll(void);

#endif /* !_VIDEO_H_ */