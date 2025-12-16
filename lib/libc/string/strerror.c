
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
static char *errno_str[EUNKNOWN] = 
{
	[ENOMEM]    = "No enough memory",
	[EMFILE]    = "Maximum number of opened files exceeded.",
	[EKERN]     = "Kernel not found",
	[EIFS]      = "Invalid filesystem",
	[EREGFS]    = "Number of registered filesystems exceeded",
	[EIO]       = "Input/Output error",
	[ENOEXEC]   = "Executable format error",
	[EINVAL]    = "Invalid value",
	[EFAULT]    = "Invalid pointer",
	[ESYSINIT]  = "Subsystem not initialized",
	[EFILE]     = "File not found",
    [EKERNLOC]  = "Kernel localization before 1MB is not supported",
    [EFS]       = "Filesystem error",
	[ENOIMP]    = "Not implemented"
};

char *strerror(int errno) 
{
	if (errno <= 0 || errno > EUNKNOWN)
	    return NULL;
	
    return errno_str[errno];
}
