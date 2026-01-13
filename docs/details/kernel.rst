
.. _kernel-info:

======
Kernel
======

Prerequisites
=============

Currently, CHB requires that the kernel meet the following conditions:

- The kernel must be compiled and linked at a physical address
  **0x100000.**

- The maximum kernel size is 128 sectors.

- The file must be an ELF executable for the i386 architecture.

Calling convention
==================

CHB transfers control to the kernel entry point using the cdecl calling
convention with regparm(1). If boot information is provided, EAX contains
a pointer to a valid msys_kern_hdr structure. Otherwise, EAX is zero.

Passing CHB obtained information to kernel
==========================================

.. code-block:: C

    struct msys_kern_hdr {
        u32t magic;
        struct device disk;
        struct memory_info mem;
        u8t vid_mode;
    };
    
**'magic'**

*optional*

This is the magic number which the kernel uses to identify that it was 
loaded with CHB, must be '0x3F8A857B'

**'disk'**

This is the information from the disk where CHB was booted. 
See :ref:`disk_info_structure` for details.

**'memory'**

This is the information obtained about the memory via INT12, INT15,E820 and INT15,88
See :ref:`memory_info_structure` for details.

**'vid_mode'**

Current BIOS video mode.

.. _memory_info_structure:

Memory info structure
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: C

    struct memory_info {
        int count;
        int lo;
        int hi;
        struct memory_block *blk;
    };

**'count'**

Number of blocks obtained in memory by INT15,E820. Note: The maximum blocks reported
is 255.

**'lo'**

Size of contigous memory starting at absolute address 0x0000, obtained by INT12

**'hi'**

Size of contigous memory starting at absolute address 0x100000, obtained by INT15,88

**'blk'**

INT15,E820 memory block buffer, points to all memory blocks obtained by INT15,E820.

**IMPORTANT NOTE:** CHB puts all the obtained blocks at absolute address 0x7B016.
See :ref:`chb_memory_map` for details.

Memory block structure
^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: C

    struct memory_block {
        uint64_t begin; 
        uint64_t len; 
        uint32_t type;
        uint32_t flag;
    };
    
**'begin'**

Start of memory section.

**'len'**

Length of memory section.

**'type'**

    0x1 -- Usable
    0x2 -- Reserved
    0x3 -- ACPI Reclaim
    0x4 -- ACPI NVS
    Other -- Not defined 

**'flag'**

Additional memory block info.

.. _disk_info_structure:

Disk info structure
~~~~~~~~~~~~~~~~~~~

.. code-block:: C

    struct device {
       u8t number;
       int state;
       bool ext;
       struct device_geometry geom;
    };
    
**'number'**

Reported BIOS drive number.

**'state'**

    0 -- Disk error
    1 -- The disk started successfully (OK)

**'ext'**

Describe whether the disk supports extensions via INT13,4X (LBA) or not.

**'geom'**

Describe the disk properties obtained by INT13,8 or INT13,48.

Disk geometry structure
^^^^^^^^^^^^^^^^^^^^^^^

The following structure describes the disk geometry and supports EDD 
protocol version 1.0 and later. This structure definition is also used 
if the disk does not use extensions (CHS).

.. code-block:: C

    struct device_geometry {
        /* 1.0+ */
        u16t size;
        u16t flags;
        u32t cyl;
        u32t head;
        u32t sector;
        u64t total_sectors;
        u16t sector_size;
        
        /* 2.0+ */
        u8t unused[39];
    } __attribute__ ((packed));
    
**'size'**

(call) size of the buffer CHB lists it as 001Eh.
(return) size of EDD

*unused in CHS mode.*

**'flags'**

*unused in CHB.*

**'cyl'**

Number of cylinders in the drive.

**'head'**

Number of heads in the drive (LBA). heads per cylinder (CHS).

**'sectors'**

Number of sectors in the drive (LBA), sectors per track (CHS).

**'total_sectors'**

Total sectors in drive.

*unused in CHS mode.*

**'sector_size'**

Size in bytes of sector, must be multiple of 512 for LBA, or 512 for CHS.

**'unused'**

Future versions of EDD protocol can have info here. *unused in CHB*