.. _install-program:

=========
install
=========

Install CHB into FAT12/16 or MBR image

Usage
-----

.. code-block:: bash 

    install [-f?V] [-b FILE] [-i FILE] [-p DIR] [--first-boot-option=FILE]
            [--force-lba] [--input=FILE] [--prefix=DIR] [--help] [--usage]
            [--version]

Arguments
---------

.. code-block:: bash 

    -b, --first-boot-option=FILE   
        Set kernel path in FS default=kernel.elf
        
    -f, --force-lba            
        Force LBA addressing mode (if the disk is HDD) default=0
        
    -i, --input=FILE           
        Input image, must be formatted for FAT12/16
        
    -p, --prefix=DIR           
        Directory where CHB was installed.
        
    -?, --help                 
        give this help list
        
    --usage                
        give a short usage message
        
    -V, --version              
        print program version

Description
-----------

‘install’ copy 'stage1' and 'loader' between LBA0 and VBR0/FSINIT

**Notes:**

- The bootloader typically jumps to address 0x7C3C, but this may vary
  depending on the definition in *'stage1/stage1.h'.*
  
- If you image is an partitioned MBR, you must reserve 2048 sectors between 
  MBR and VBR0. Additionally, the FAT32 partition used must be marked as 
  'bootable' (TODO: improve this!)

- If you image is a floppy you must reserve 45 sectors between LBA0 and FS sector 
  start (for FAT12/16 is the -R option in 'mkfs.fat'
  
- For FAT32 CHB doesn't support superfloppies.
  

Examples
--------
.. code-block:: bash 
    
    # install in FAT12/16 image
    ./build/install --first-boot-option=boot/kernel.elf --input=floppy16.img --prefix=build/
    
    #install in MBR 
    ./build/install --force-lba --first-boot-option=boot/kernel.elf --input=disk.img --prefix=build/

License
-------

This program can be redistributed under the terms of the MIT License.
See './COPYING' file for more information.
