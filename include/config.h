
/*
 * config.h -- config options for CHB
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_ 1

/* print memory reported by INT12 and INT15,E820 */
#define CHB_PRINT_MEMORY_INFO 0

/* First option that CHB can boot from FS. */
#define CHB_FIRST_BOOT_OPTION "kernel.elf"

/* DO NO EDIT: CHB VERSION */
#define CHB_VERSION_STRING "0.08-prealpha"

/* DO NO EDIT: CHB VERSION NUMBERS */
#define VERSION_MAJOR 0
#define VERSION_MINOR 8
#define VERSION_REV 0
#define VERSION_PACKED VERSION_REV | (VERSION_MINOR << 8) | (VERSION_MAJOR << 16)

#endif /* !_CONFIG_H_ */