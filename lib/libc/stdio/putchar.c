
/*
* putchar.c -- CHB putchar function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/stdio.h>
#include <CHB/video.h>

void
putchar(int c) {
   video_driver_putchar(0x0, 0x7, c);
}