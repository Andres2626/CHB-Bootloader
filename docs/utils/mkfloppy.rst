mkfloppy
========

Create empty floppy image compatible with standard sizes of IMB/PC
reported by INT13,8.

Usage
-----

.. code-block:: bash 

    ./mkfloppy <*arg1*> <*arg2*>

Arguments
---------

*arg1:* Number of total sectors in floppy image (`see supported
formats <#supported-formats>`__)

*arg2:* Output file

Description
-----------

*‘mkfloppy’* creates an empty floppy image using *‘dd’.* The size of
floppy image depends of reported with INT13,8 in IBM/PC compatible
machines.

The default sector size is: **512 bytes.**

Supported formats
-----------------

The following is a list of supported floppy formats (according INT13,8):

- **360 KB –** 720 sectors.

- **720 KB –** 1440 sectors.

- **1.44MB –** 2880 sectors

- **1.88MB –** 5760 sectors

Examples
--------

Create 1.44MB floppy image:

.. code-block:: bash

    ./mkfloppy 2880 myfloppy.img

Error handling:
---------------

If the sector number is not compatible with `supported
formats <#supported-formats>`__, the program prints list of formats and
exit. The output file is **obligatory** otherwise the program print
error message and exit.

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more information.
