
/*
 * macros.h -- C/ASM macros
 *
 * Copyright (C) 2021 - 2025 andres26
 *
 * This file is distributed under the terms of the MIT license.
 */

#ifndef _ASM_MACROS_H_
#define _ASM_MACROS_H_ 1

#if defined(STAGE1_FILE) || defined(LOADER_FILE)
/* stage1 real mode print */
#define MSG(x)                                                                                                         \
   mov $x, % si;                                                                                                       \
   call _print
#endif /* STAGE1_FILE */

#if defined(ASM_FILE)
#define GLOBAL(x)                                                                                                      \
   .global x;                                                                                                          \
   x:
#define SECTION(x) .##x
#else
#define GLOBAL(x)  x
#define SECTION(x) x
#endif /* ASM_FILE */

#define VARIABLE(x) GLOBAL(x)
#define FUNCTION(x) GLOBAL(x)

#endif /* !_ASM_MACROS_H_ */