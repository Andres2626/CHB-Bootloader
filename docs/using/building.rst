============
Building CHB
============

This is the complete guide to building CHB

Contents of main repository
---------------------------

    **'docs/'**

    All CHB documentation.

    **'include/'**

    Here are all the files that CHB includes in its source code.

    **'lib/'**

    All libraries used by CHB.

    **'loader/'**

    'loader' program.

    **'scripts/'**

    Programs used in compilation.

    **'stage1/'**

    'bootsector' program.

    **'util/'**

    CHB host utilities.

    **'ChangeLog*'**

    List of changes in the code.

    **'COPYING**'**

    CHB MIT license

    **'README'**

    Please read this!

    **'TODO'**

    Project roadmap.

    **'build.sh'**

    CHB Build system main script.

    **'.gitignore'**

    git ignored files and directories.

    **'makefile.mk'**

    CHB main makefile.

.. _building_prerequisites:

Prerequisites
-------------

Before building CHB from source, please verify that the following tools/dependencies 
are installed and up to date on your host.

================   ===============  ============================
Program name       Tested with      Command to check the version
================   ===============  ============================
GNU GCC            13.4.0           gcc --version
GNU Binutils       2.45.1           ld --version
GNU Make           4.4.1            make --version
GNU Coreutils      9.0              dd --version
bash               5.2.21           bash --version 
python             3.12.2           python --version
Sphinx\ [#n1]_     8.2.3            sphinx-build --version
mkfs.fat           4.2              mkfs.fat --version
GNU mtools         4.0.35           mtools --version
GDB\ [#n2]_        16.2             gdb --version
x86-qemu\ [#n3]_   8.2.90           qemu-system-i386 --version
bochs\ [#n4]_      2.7              bochs --version
git\ [#n5]_        2.51.0           git --version          
================   ===============  ============================

.. [#n1] 'Sphinx' is only needed for build CHB documentation
.. [#n2] 'GDB' is only used for debug CHB.
.. [#n3] 'qemu' is only used for running CHB (optional).
.. [#n4] 'bochs' is only used for running CHB (optional).
.. [#n5] 'git' is only used for obtain CHB (optional).

**NOTES** 

- It is recommended that the prefix be defined for the cross
  compiler because on certain types of hosts (*e.g. cygwin, msys2 or some
  linux environment on windows*) cross compilation is not allowed for ELF
  executables which is necessary to compile CHB correctly.

- Older version are not tested. (I encourage anyone to try it with previous 
  versions)

Obtaining CHB
-------------

For obtain CHB from **oficial repository** run:

.. code-block:: bash

    git clone https://github.com/Andres2626/CHB-Bootloader.git && cd ./CHB-Bootloader

Building CHB from script
------------------------

Please note that there are 2 possible files that build CHB from source:
using Makefile or build.sh script. Building CHB directly with the
makefile is not recommended because the build script defines a series of
variables necessary to build CHB with cross-compilation.

'./build.sh' has the following options, so use them with care especially
when defining a cross-compilation interface.

Usage of build.sh

    ' build.sh -- build/clean system for CHB'

    Usage: build.sh [t] [h] [b] [d] [c] [?] [v]

    Configure CHB for build/clean

    For use this program build.sh detect if './Makefile' exists. Please note that to
    compile CHB only 'build.sh' is used because build.sh sets up all initial variables and
    enables the interface for the cross compiler.

    Initially to use 'build.sh' it is necessary to configure the program mode, which
    are:

    - build -- compile CHB from source

    - clean -- delete all CHB compiled files

    In addition to this YOU CAN configure the prefix for both host compilers
    and cross compilers It is recommended that the prefix be defined for the
    cross compiler because on certain types of hosts (e.g. cygwin, msys2 or
    some linux environment on windows) cross compilation is not allowed for
    ELF executables which is necessary to compile CHB correctly.

    Notes:

    - If you're going to compile CHB for an x86_64 machine, build.sh automatically
      force '-m32' flag in gcc and '-melf_i386' in CHB doesn't support long mode.

    - Note that CHB only supports x86-based machines. (TODO: implement
      this.
      
    Options:

    -t, --target=[TARGET_PREFIX]

        Sets the cross-compiler prefix so that the compiler you specified is
        used when compiling.

        It is highly recommended to define the cross-compilation interface
        because some systems do not support compiling to generate ELF format
        files.

    -h, --host=[HOST_PREFIX]

        Set the host machine.

        Unlike --target, it is not necessary to specify the host because the
        script automatically detects the host if it is not specified.

    -b, --build

        Build CHB from source.

    -c, --clean

        Clean compiled CHB files.

    -d, --docs=[MODE]

        Build all CHB documentation with Sphinx, current modes supported are:

        - html -- Generate HTML pages
        - latex -- Generate .tex file
        - pdflatex -- Convert .tex to pdf
        - htmlhelp -- Generate windows CHM format
        - text -- Generate text file
        - clean -- Clean documentation generated files

    -v, --version

        Print program version

    -?, --help

        Print Help
