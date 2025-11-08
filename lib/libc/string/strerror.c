
/*
* strerror.c -- strerror function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include "lib/libc/string.h"
#include "lib/libc/errno.h"

/* Supported error numbers */
static char *errno_str[EUNKNOUN] = 
{
	[ESINV]    = "Invalid disk sector",
	[EREAD]    = "Read error",
	[EDISK]    = "Disk error",
	[EDEVINT]  = "Disk driver is not initialized",
	[ENOMEM]   = "No enough memory",
	[EMFILE]   = "Maximum number of opened files exceeded",
	[EKERN]    = "Kernel not found",
	[EFATTR]   = "Invalid file attribute",
	[EKERNLOC] = "Kernel localization in before 1MB is not supported",
	[EMEMINIT] = "Memory driver is not initialized",
	[EIFS]     = "Invalid filesystem",
	[ENOIMP]   = "Not implemented"
};

char *strerror(int errno) 
{
	if (errno <= 0 || errno > EUNKNOUN)
	    return NULL;
	
    return errno_str[errno];
}
