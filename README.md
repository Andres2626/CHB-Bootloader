**CHB LEGACY BOOT TEST (pre-0.01, 2023)**

This is an early version of CHB, uploaded for historical purposes.

This version was created in 2023, long before the official version 0.01 of this repository. It tested a hybrid 'stage 1' between C 
and ASM (configuring the GDT, real/protected mode, etc.). 'Stage 1' decompressed 'stage 2' (which was never implemented) and only 
converted one absolute sector to CHS.

This branch is not intended to be merged with 'master' and is read-only. Its purpose is to show the old design and how CHB evolved from 
its initial form to its current state.

**NOTE OF BINARIES**

This version does not have compiled binaries, but the makefile compiles without issue; you can try it yourself and study its code.

**LICENSE**

The day this code was uploaded it was unlicensed, now it has the MIT license

2026-01-10 Andres26
