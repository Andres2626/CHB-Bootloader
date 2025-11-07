
/*
 * ctype.h -- ctype prototype functions
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _CTYPE_H_
#define _CTYPE_H_ 1

#include "const.h"

PROTOTYPE(char toupper, (int c));
PROTOTYPE(int isupper, (int c));
PROTOTYPE(char tolower, (int c));
PROTOTYPE(int islower, (int c));

#endif /* !_CTYPE_H_ */