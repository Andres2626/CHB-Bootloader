========
mkfloppy
========

Create empty floppy image compatible with standard sizes of IMB/PC
reported by INT13,8.

Usage
-----

.. code-block:: bash 

    ./mkfloppy <sectors> <output>

Arguments
---------

*sectors:* Number of total sectors in floppy image. See :ref:`supported-floppy-formats`

*output:* Output file

Description
-----------

*‘mkfloppy’* creates an empty floppy image using *‘dd’.* The size of
floppy image depends of reported with INT13,8 in IBM/PC compatible
machines.

See :ref:`supported-floppy-formats` for more information.

The default sector size is: **512 bytes.**

Examples
--------

.. code-block:: bash
    
    # Create 1.44MB floppy image:
    ./mkfloppy 2880 myfloppy.img

Error handling:
---------------

If the sector number is not compatible with :ref:`supported-floppy-formats`, 
the program prints list of formats and exit. The output file is **obligatory** 
otherwise the program print error message and exit.

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more information.
