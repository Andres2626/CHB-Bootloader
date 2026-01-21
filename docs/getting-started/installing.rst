
.. _installing-chb:

==============
Installing CHB
==============

At the moment CHB doesn't support CDFS. TODO: implement ISO9960 support.

.. _supported-filesystems:

Supported Filesystems
---------------------

Currently, CHB supports the following filesystems; your kernel must be 
installed on one of the following:

#. FAT12/16\ [#n1]_
#. FAT32\ [#n2]_

.. [#n1] Supported only for floppies.
.. [#n2] Supported for partitioned MBR image (superfloppy is not suported)

Image layout
------------

For all disk/floppy image CHB must have an special space for store 'loader'
here. CHB is usually installed in this way\ [#n3]_

.. code-block:: text
    
    stage1 (LBA0) -> loader (LBA1 .... LBAN) -> FS begin or VBR0

**IMPORTANT NOTES:** 

- For floppies CHB install 'stage1' and 'loader' before begin of any FS structure
- For disks (must be have MBR) CHB install 'stage1' and 'loader' between 
  MBR and VBR0 (The minimum size of MBR and VBR0 is 2048)
   
See :ref:`install-program` for more information.

.. [#n3] For FAT12/16 filesystems the 'stage1' is combined with the information 
   from the FAT bootsector, so that CHB can be installed there, at least 45 sectors 
   must be reserved.

Creating empty MBR partitioned image
------------------------------------

CHB does not currently offer a utility to create a disk image with an MBR 
partition, so you will have to create them yourself using the 'fdisk' 
command or similar.

**IMPORTANT NOTES:** 

- To install CHB and be able to detect the FS, the partition 
  of the FS to be used must be marked as bootable, in addition to this, 2048 
  sectors must be reserved between the MBR and the VBR0.

- It doesn't matter which FAT32 partition is on the disk, as the 
  first bootable FAT32 partition is detected. (TODO: improve this!)

- If you create an image with 'mkfs.fat' (or any FAT utility) it is not 
  necessary to reserve sectors, I recommend that before assembling the partitioned 
  disk, you create a FAT32 image with the exact size of the partition and copy this 
  to the partition marked as FAT32 on the disk. This is because CHB automatically 
  increments the sector where the VBR begins each time it reads the disk.

Creating empty floppy image
---------------------------

CHB offers a utility for creating blank floppy disk images. **All**
images generated with ‘\ *mkfloppy*\ ’ have the exact number of sectors
required to comply with INT13.8 on IBM/PC machines. This is because QEMU
or BOCHS will fail to read the floppy disk if it doesn't have a specific
number of sectors, and it's also necessary for mkfs.fat to recognize it
as a valid image.

Please note that 'stage1' loads 'loader' by locating after the LBA0.
'stage1' does not function as a locator within the FS; that is the 
responsibility of 'loader'.

**MKFS.FAT IMPORTANT NOTE**: If you create an image with 'mkfs.fat' (or
any FAT utility) it is important to reserve a number of sectors
greater than or equal to 45 so that 'loader' can be stored there and
'stage1' can find it.

To create an empty floppy image, it is necessary to invoke the CHB `mkfloppy` 
command.

After invoking the command, it should create an empty image with the
specified number of sectors.

**CAUTION:** *The program does not detect whether the image is empty or
not, so it rewrites all the information contained in it.*

Done! Now you have an empty image ready to install and run CHB within
it.

Installing CHB in any floppy or disk image
------------------------------------------

To install CHB on LBA 0, you need to run the CHB 'install' program.

See :ref:`install-program` for more information.

NOTE OF HOST BINARIES
---------------------

Because the binaries for all utilities are not available in the main
repository, you will need to compile some of these utilities yourself
and adjust the *\`--prefix\`* option. Then, use the target binaries
provided in the repository and move them to the *\`prefix\`* directory.
