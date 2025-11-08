
/*
* ernno.c -- CHB error library
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/errno.h"

int errno = 0;

void set_errno(int error) 
{
	errno = error;
}
