mkfs12
======

Make FAT12 CHB bootsector.

Usage
-----

.. code-block:: bash

    ./mkfs12 [-i] [-p]

Options
-------

-i, --input-fat-img=[FILE]

   Input formatted FAT12 image. This image is edited and at the end it
   remains with the boot sector of CHB and the BPB of FAT.

-p, --prefix=[PATH]

   Directory where CHB was installed. The program looks for stage1.IMG.

Description
-----------

*‘mkfs12’* creates a bootsector containing the CHB *'stage1'* code. The
input image must be previously formatted as *'mkfs.fat'* and configured
with at least 45 reserved sectors to allow for the installation of the
bootloader after the bootsector via `install12 <install12.rst>`__
command.

**Notes:**

- The bootloader typically jumps to address 0x7C3C, but this may vary
  depending on the definition in *'stage1/stage1.h'.*

- The image must have the number of sectors specified by INT13.8; `see
  supported floppy formats <mkfloppy.rst>`__ for more info.

- The end of the bootsector must contain the signature 0xAA55 for the
  program to recognize it.

Examples
--------

.. code-block:: bash

    ./mkfs12 --input-fat-image=floppy.img --prefix=/boot/CHB/

Error handling
--------------

The following is a list of things that *'mkfs12'* may fail to do:

- The input image or prefix is not defined by user.

- The ‘stage1’ doesn’t exist in prefix directory.

- Input image could not be opened.

- Input image doesn’t have **obligatory** sectors (`see supported floppy
  formats <mkfloppy.rst>`__)

- Input image doesn’t have enough space for BPB.

- Reserved sectors less than 45.

- Input image contains invalid signature (0x55AA)

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more information.
