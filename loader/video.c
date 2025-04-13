
/*
* video.c -- simple video controller for 80x25 modes
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/port.h>
#include <CHB/types.h>
#include <CHB/video.h>

static uint8_t cursor_start = 0;
static uint8_t cursor_end = 0;
static uint16_t cursor_index = 0;
static uint8_t* vid_mm = VIDEO_DRIVER_MM;
static uint8_t vid_mode = 0;

void
video_driver_init(uint8_t mode) {
   vid_mode = video_driver_set_mode(mode);
   video_driver_cursor_enable(14, 15);
   video_driver_cls(0x0, 0x7);
}

void
video_driver_cursor_enable(uint8_t start, uint8_t end) {
   port_outb(0x3D4, 0x0A);
   port_outb(0x3D5, (port_inb(0x3D5) & 0xC0) | start);
   port_outb(0x3D4, 0x0B);
   port_outb(0x3D5, (port_inb(0x3D5) & 0xE0) | end);
   cursor_start = start;
   cursor_end = end;
}

void
video_driver_cursor_disable(void) {
   port_outb(0x3D4, 0x0A);
   port_outb(0x3D4, 0x20);
}

void
video_driver_cursor_gotoindex(uint16_t index) {
   port_outb(0x3D4, 0x0F);
   port_outb(0x3D5, (uint8_t)(index & 0xFF));
   port_outb(0x3D4, 0x0E);
   port_outb(0x3D5, (uint8_t)((index >> 8) & 0xFF));
   cursor_index = index;
}

void
video_driver_cursor_gotoxy(uint8_t x, uint8_t y) {
   video_driver_cursor_gotoindex(y * 80 + x);
}

uint16_t
video_driver_cursor_getindex(void) {
   return cursor_index;
}

void
video_driver_putchar(uint8_t background, uint8_t foreground, char c) {
   uint16_t index = video_driver_cursor_getindex();
   uint16_t color = (background << 4) | (foreground & 0x0F);
   switch (c) {
      case '\b':
         index--;
         *(vid_mm + index * 2) = 0x0;
         *(vid_mm + index * 2 + 1) = color;
         break;
      case '\v':
      case '\f': index += 80; break;
      case '\t': index = (index + 8) & ~(8 - 1); break;
      case '\n': index -= index % 80 - 80; break;
      case '\r': index -= index % 80; break;
      default:
         *(vid_mm + index * 2) = c;
         *(vid_mm + index * 2 + 1) = color;
         index++;
         break;
   }
   video_driver_cursor_gotoindex(index);
   video_driver_scroll(background, foreground);
}

void
video_driver_cls(uint8_t background, uint8_t foreground) {
   video_driver_cursor_gotoxy(0, 0);
   for (int i = 0; i < (80 * 25); i++) {
      video_driver_putchar(background, foreground, ' ');
   }
   video_driver_cursor_gotoxy(0, 0);
}

void
video_driver_scroll(uint8_t background, uint8_t foreground) {
   uint16_t color = (background << 4) | (foreground & 0x0F);
   uint8_t cursor_x = video_driver_cursor_getindex() % 80;
   uint8_t cursor_y = (video_driver_cursor_getindex() - cursor_x) / 80;
   uint16_t* loc = (uint16_t*)0xb8000;
   uint8_t attributeByte = color;
   uint16_t blank = 0x20 | (attributeByte << 8);
   if (cursor_y >= 25) {
      for (int i = 0; i < 24 * 80; i++) {
         loc[i] = loc[i + 80];
      }
      for (int i = 24 * 80; i < 25 * 80; i++) {
         loc[i] = blank;
      }
      cursor_y = 24;
   }
   video_driver_cursor_gotoxy(cursor_x, cursor_y);
}