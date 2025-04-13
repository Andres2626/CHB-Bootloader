
/*
 * port.h -- x86 port prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _PORT_H_
#define _PORT_H_ 1

#include <CHB/types.h>

void port_outb(uint16_t port, uint8_t value);
uint8_t port_inb(uint16_t port);

#endif /* !_PORT_H_ */