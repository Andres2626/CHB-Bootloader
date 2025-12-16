
/*
 * exec.h -- executable load driver
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _EXEC_H_
#define _EXEC_H_ 1

#include "types.h"
#include "const.h"

int elf_load(void *input, u32t size, void **entry_point);

#endif /* !_EXEC_H_ */