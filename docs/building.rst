Building CHB
============

This is the complete guide to building CHB

Contents of main repository
---------------------------

- *docs/ --* All CHB documentation.

- *include/* -- '*.h*' files.

- *lib/* -- tiny implementation of libc (*lchb*)

- *loader/* -- kernel loader.

- *scripts/* -- programs used in compilation.

- *stage1/* -- bootsector program.

- *util/* -- CHB utilities.

- *ChangeLog*

- *COPYING*

- *.clang-format*

- *build.sh* -- build system main script.

- *Makefile*

- *README*

Prerequisites
-------------

Build tools:

- GCC and Binutils (I use 13.2.0 and 2.41)

- GNU Make

- GNU Coreutils (dd, mkdir, etc.)

- Mtools or any FAT-compatible disk image editor.

- mkfs.fat (FAT formatting).

Debug and run tools:

- GDB.

- An x86-based virtual machine.

**NOTE**! It is recommended that the prefix be defined for the cross
compiler because on certain types of hosts (*e.g. cygwin, msys2 or some
linux environment on windows*) cross compilation is not allowed for ELF
executables which is necessary to compile CHB correctly.

Building CHB from script
------------------------

Please note that there are 2 possible files that build CHB from source:
using Makefile or build.sh script. Building CHB directly with the
makefile is not recommended because the build script defines a series of
variables necessary to build CHB with cross-compilation.

'./build.sh' has the following options, so use them with care especially
when defining a cross-compilation interface.

Usage of build.sh

.. code-block:: txt 

    ' build.sh -- build/clean system for CHB'

    Usage: $0 [t] [h] [b] [d] [c] [?] [v]

    Configure CHB for build/clean

    For use this program $0 detect if ./Makefile exists. Please note that to
    compile CHB only $0 is used because $0 sets up all initial variables and
    enables the interface for the cross compiler.

    Initially to use $0 it is necessary to configure the program mode, which
    are:

    - build -- compile CHB from source

    - clean -- delete all CHB compiled files

    In addition to this YOU CAN configure the prefix for both host compilers
    and cross compilers It is recommended that the prefix be defined for the
    cross compiler because on certain types of hosts (e.g. cygwin, msys2 or
    some linux environment on windows) cross compilation is not allowed for
    ELF executables which is necessary to compile CHB correctly.

    Notes:

    - If you're going to compile CHB for an x86_64 machine, $0 automatically
      compiles it for i386 because CHB doesn't support long mode.

    - Note that CHB only supports x86-based machines. (TODO: implement
      this.)

    - If you plan to use the cross-compilation interface, you must have the
      GCC and binutils compilers installed on your system first.

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

    build all CHB documentation with Sphinx, current modes supported are:

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
