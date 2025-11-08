
/*
 * port.h -- x86 port prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _PORT_H_
#define _PORT_H_ 1

#include "types.h"
#include "const.h"

PUBLIC void _ASM port_outb(u16t port, u8t value);
PUBLIC u8t _ASM port_inb(u16t port);

#endif /* !_PORT_H_ */