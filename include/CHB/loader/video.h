
#ifndef _VIDEO_H_
#define _VIDEO_H_ 1

#include <CHB/types.h>

void video_init();
void video_set_mode(uint8_t mode);
void video_gotoxy(uint8_t x, uint8_t y);
uint8_t video_getxy();
void video_putchar(int color, char c);
void video_scroll(int n);
void video_clear(int color);

#endif /* !_VIDEO_H_ */