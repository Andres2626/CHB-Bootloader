
/*
* strerror.c -- strerror function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/errno.h>
#include <CHB/string.h>

char*
strerror(int errno) {
   switch (errno) {
      case ERR_SECTOR_INV: return "Invalid disk read/write sector";
      case ERR_DISK_READ: return "Disk reading error";
      case ERR_DISK_ERR: return "Disk error";
	  case ERR_DEVICE_INIT: return "Device is not initialized";
	  case ERR_NO_MEM: return "Not enough memory";
	  case ERR_TOO_FILES: return "Maximum number of opened files exceeded";
	  case ERR_KERN_NO: return "Kernel not found in filesystem";
	  case ERR_KERN_LOC: return "Kernel localization in before 1MB is not supported";
      case ERR_NO: return "No error.";
      case ERR_UNKNOUN:
      default: return "Unknoun error";
   }

   return NULL;
}
