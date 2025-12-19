CHB DOCUMENTATION
=================

CHB (Computers Help Bootloader) is a two-stage bootloader targeting the x86 architecture, primarily 
intended for kernel bootstrapping and low-level debugging. The current implementation is minimalistic 
and features a FAT12 file system parser, addressing a limitation of the previous version which 
lacked support for loading kernels across non-contiguous (dynamic) sectors.

Testing has been limited to x86 virtualized environments such as QEMU and Bochs, though it is 
expected to function correctly on any compliant x86 hypervisor or emulator.

Copyright (C) andres26 2021-2025

**ALL** code may be used or redistributed under the terms of the MIT License.

----

.. contents::
   :local:
   :depth: 2


----

.. toctree::
    :caption: Contents
    :maxdepth: 2
    
    
    contributing
    code-of-conduct
    philosophy
    technical-aspects
    building
    build-images
    installing
    debugging
    error-handling
    kernel
    filesystem
    memory-map
    have-fun
    contact
    utils/index
    appendix/minimal-kernel-example
    appendix/clib
    appendix/license