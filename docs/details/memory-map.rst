.. _chb_memory_map:

==============
CHB Memory Map
==============

=============   ===========  ===========  ====================================
Begin address   End address  Size         Description
=============   ===========  ===========  ====================================
0x00000         0x9FFFF      0x9FFFF      Lower memory
0xA0000         0xFFFFF      0x5FFFF      Upper memory
0x00000         0x07BFF      0x07BFF      BIOS, RM-IDT, BDA and INT13 scratch
0x07C00         0x07E00      0x00200      Stage1 (bootsector) and stack
0x07E01         0x07FFF      0x001FE      Reserved
0x08000         0x102FF      0x082FD      Loader (64 sectors) and stack
0x10300         0x12FFF      0x02CFF      Loader FS driver buffer
0x13000         0x23000      0x10000      Kernel load buffer
0x23001         0x7B016      0x58015      Unused
0x7B016         0x7CFFE      0x01FE8      INT15,E820 memory block buffer
0xA0000         0xFFFFF      0x5FFFF      Video memory and mapped hardware
0x100000                                  Extended memory (kernel)
=============   ===========  ===========  ====================================
