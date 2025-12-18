*CHB MKFLOPPY PROGRAM*
======================

Create empty floppy image compatible with standard sizes of IMB/PC
reported by INT13,8.

*Usage*
-------

./mkfloppy <*arg1*> <*arg2*>

*Arguments*
-----------

*arg1:* Number of total sectors in floppy image (`see supported
formats <#_Supported_formats:>`__)

*arg2:* Output file

*Description*
-------------

*‘mkfloppy’* creates an empty floppy image using *‘dd’.* The size of
floppy image depends of reported with INT13,8 in IBM/PC compatible
machines.

The default sector size is: **512 bytes.**

*Supported formats*
-------------------

The following is a list of supported floppy formats (according INT13,8):

- **360 K\ B – ** *720 sectors.*

- **720 K\ B – ** *1440 sectors.*

- **1.4\ 4\ MB – ** *2880 sectors*

- **1.\ 88\ MB – ** *5760 sectors*

*Examples*
----------

Create 1.44MB floppy image:

./mkfloppy 2880 myfloppy.img

*Error handling:*
-----------------

If the sector number is not compatible with `supported
formats <#_Supported_formats>`__, the program prints list of formats and
exit. The output file is **obligatory** otherwise the program print
error message and exit.

*License*
---------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more info
