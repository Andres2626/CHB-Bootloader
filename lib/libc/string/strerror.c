
/*
* strerror.c -- strerror function
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <CHB/errno.h>
#include <CHB/string.h>

int errno = 0;

char*
strerror(int errno) {
   switch (errno) {
      case ESEC: return "Invalid sector";
      case EREAD: return "Read error";
      case EDEV: return "Device error";
      case ENO: break;
      case EUNKNOUN:
      default: return "Unknoun error";
   }

   return NULL;
}
