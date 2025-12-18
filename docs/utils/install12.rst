CHB INSTALL12 PROGRAM
=====================

Install CHB loader in formatted FAT12 image.

Usage
-----

./install12 <arg1> <arg2>

Arguments
---------

*arg1*: Directory where CHB was installed. The program looks for
stage1.IMG

*arg2:* Output file (previously formatted with ‘\ *mkfs.fat’* and
`mkfs <mkfs12.rst>`__)

Description
-----------

*‘install12’* copy CHB loader before bootsector so that *‘stage1’* can
jump to it.

**Notes:**

- The bootloader typically jumps to address 0x7C3C, but this may vary
  depending on the definition in *'stage1/stage1.h'.*

- The image must have the number of sectors specified by INT13.8; `see
  supported floppy formats for more information. <mkfloppy.rst>`__

Examples
--------

./install12 /boot/CHB CHB.img

Error handling:
---------------

The following is a list of things that *'mkfs12'* may fail to do:

- The prefix and output image were not specified.

- Output image doesn’t have **obligatory** sectors (`see supported
  floppy formats <mkfloppy.rst>`__)

- Output image or loader file not found by ‘\ *dd*\ ’

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more info
