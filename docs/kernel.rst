Kernel
======

The following conditions are needed to load the kernel:

- The kernel must be compiled and linked at a physical address
  **0x100000.**

- The maximum kernel size is 128 sectors.

- The file must be an ELF executable for the i386 architecture.

- The kernel must be located in the FS like this: **'/kernel.ELF'**

- The data structure that CHB passes to the kernel is optional; it is
  activated with *'push %eax'* after the kernel is configured.

Kernel HDR
----------

This is the structure that CHB passes to the kernel for later use.

- u32t magic number, must be 0x3F8A857B

- disk structure.

- memory info structure.

- current video mode.
