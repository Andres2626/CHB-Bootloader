=============================================
Technical aspects and general characteristics
=============================================

**Simple build script** 

    For enable support for cross-compiling, CHB implements the build script, 
    this configure important variables used by makefile (e.j. Compiler prefix) 
    and runs makefile for build CHB. The script does not validate tool versions. 
    
    See :ref:`building_prerequisites` for more information.

**Simple and dynamic kernel loading** 

    This version add support for very simple FS driver, 'stage1' pass control to 
    'loader' and execute kernel. At the moment, CHB report to kernel, memory size
    (via BIOS interrupts), disk info (MBR, boot disk and extensions). CHB only 
    support kernel ELF ejecutables.
    
    See :ref:`kernel-info` for more info.

**Simple installing**

    This version has a new 'install' utility that currently installs 'stage1' and 
    'loader' on floppies and disks following a very specific layout. At the moment, 
    it is heavily dependent on the implemented filesystems, but sufficient to load 
    the kernel into memory using a specific partition.
    
    See :ref:`installing-chb` and :ref:`install-program` for more information.
    
    CHB fully supports floppy disks, but they have to be of a very specific size 
    (agreed upon according to INT13.8 on the IBM/PC)
    
    See :ref:`supported-floppy-formats` for more information.
    
**Protected mode**

    CHB changes to protected mode before call kernel,At the moment CHB doesn't 
    support long mode, if you will load kernel using CHB, you must compile it 
    with '-m32' compiler option.

**two-stage design bootloader** 

    CHB follows a two-stage bootloader design. This approach simplifies the 
    overall implementation by avoiding the size constraints of the MBR and 
    eliminating the need to create a  dedicated 'stage1' loader for each 
    supported filesystem.

**Filesystem** 
    
    At the moment, CHB supports limited number of filesystems. The philosophy 
    behind these is to upload and download one file at a time, thus avoiding 
    the need to create a complete VFS to use the filesystems.
    
    See :ref:`supported-filesystems` for more information.

**Memory** 

    CHB obtains the memory map using several BIOS calls such as INT12, 
    INT15,88 and INT15,E820, which are passed to the kernel before it 
    is executed for later use.
    
    For view memory map layout please see: :ref:`chb_memory_map`.

**ELF Loader** 

    For load kernels CHB use ELF executable format, at the moment this 
    loader is very tiny implementation, but sufficient for load and execute 
    your kernel in memory. Relocations and paging are not supported.
    
**Boot process**

    CHB loads itself in a very strict way, this ensures forward and backward 
    compatibility between different versions. Currently, it is only written 
    for x86-based machines; other architectures are on the way!
    
    See :ref:`x86-boot-process` for more information.
    
**Complete documentation**
    
    From now on, Sphinx will be used to build the CHB project documentation. Much of it has been 
    rewritten, and new sections have been added. All documentation is now located in the 'docs/' 
    directory.

    Sphinx supports a wide range of formats for rendering documentation. CHB currently supports the 
    following formats:

    - html      -- Generate HTML pages
    - latex     -- Generate .tex file
    - pdflatex  -- Convert .tex to pdf
    - htmlhelp  -- Generate windows CHM format 
    - text      -- Generate text file

