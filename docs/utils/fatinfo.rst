CHB FATINFO PROGRAM
===================

View information of FAT12 formatted image.

Usage
-----

.. code-block:: bash

    ./fatinfo <*arg1*>

Arguments
---------

*arg1:* Input file

Description
-----------

*‘fatinfo’* Prints the information from a FAT12 image as provided by the
bootsector.

Examples
--------

.. code-block:: bash

    ./fatinfo fat12.img

Error handling:
---------------

The following is a list of things that *'mkfs12'* may fail to do:

- No arguments provided.

- Input file is not found.

- Input image contains invalid signature (0x55AA)

- BPB size is invalid.

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more information.
