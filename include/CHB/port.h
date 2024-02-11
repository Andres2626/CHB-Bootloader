
#ifndef _PORT_H_
#define _PORT_H_ 1

#include <CHB/types.h>

void port_outb(uint16_t port, uint8_t value);
uint8_t port_inb(uint16_t port);

#endif /* !_PORT_H_ */