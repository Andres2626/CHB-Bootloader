**CHB Notes and manual** 

***Version: 0.03-prealpha*** 

0. **Introduction**  

   CHB is a two-stage bootloader targeting the x86 architecture (x86\_64 and i386), primarily intended for kernel bootstrapping and low-level debugging. The current implementation is minimalistic and features a FAT12 file system parser, addressing a limitation of the previous version which lacked support for loading kernels across non- contiguous (dynamic) sectors. 

   CHB stands for (C)omputers (H)elp (B)ootloader. As of now, it supports only raw flat binary kernels and does not implement support for ELF-formatted executables. Testing has been limited to x86 virtualized environments such as QEMU and Bochs, though it is expected to function correctly on any compliant x86 hypervisor or emulator. 

1. **Copyright notice** 

   All bootloader code is (C) Andres26 2021-2025, but the code can be redistributed under the terms of the MIT License. 

   The license is disclosed in the COPYING file. See this file for more information. 

2. **Contribute** 

   Anyone is FREE to contribute to the CHB project if they are interested. I encourage anyone interested in this project to contribute to it. 

   You can contribute as long as you comply with the terms given by the MIT license. 

3. **Prerequisites** 

   For building CHB you need: 

- *GCC and Binutils (I use 13.2.0 and 2.41 respectively) Note: It is recommended that the prefix be defined for the cross compiler because on certain types of hosts (e.g. cygwin, msys2 or some linux environment on windows) cross compilation is not allowed for ELF executables which is*  

  *necessary to compile CHB correctly.* 

- *GNU Make* 
- *GNU Coreutils (dd, mkdir, etc.)* 
- *Mtools or any FAT-compatible disk image editor.* 
- *mkfs.fat (for format fat12 image)* 

For run or debug CHB you need: 

- *GDB* 
- *An bassed x86 virtual machine (qemu or bochs are fine)*
4. **Hardware testing.** 

   CHB has not been tested on physical hardware. Due to its minimal design and limited feature set, it does not implement many of the basic functions typically required by modern systems. However, no special requirements are needed to attempt booting CHB on real hardware. 

   If you are interested in testing CHB on a physical machine, doing so is encouraged, as it may help uncover bugs and improve code reliability through real-world testing. 

   **DISCLAIMER:** Anyone is free to test CHB on physical hardware at their own risk. The CHB project and its contributors are not responsible for any potential damage to hardware or data loss resulting from such testing. 

5. **Contents of main repository** 

   The following files are located in the main repository: 

- *include/ -> Contains all CHB headers* 
- *lib/ -> Contains all library sources, primarily implementation of libc* 
- *loader -> Contains source of loader (stage2)* 
- *scripts -> Miscellaneous scripts used in CHB compilation.* 
- *stage1 -> Contains source of stage1.* 
- *util -> Contains source of tools of CHB.* 
- *.clang-format -> C code styler.* 
- *build.sh -> This is CHB build system.* 
- *Makefile -> This is principal makefile for CHB.* 
- *README -> This file.* 
6. **CHB Philosophy** 

   **CHB** is an independent bootloader project that is not based on conventional bootloaders such as GRUB or LILO, nor does it follow a Unix-based design. The core philosophy behind CHB is simplicity—offering a lightweight alternative to traditional bootloaders—while also avoiding licensing constraints commonly associated with projects under the GPL. 

7. **Technical aspects and general characteristics** The following are general characteristics of CHB: 
1. In earlier versions, CHB did not support dynamic kernel loading from disk. This limitation has been addressed with the introduction of a FAT12 file system controller, enabling flexible loading of the kernel from variable disk locations. 
1. CHB follows a two-stage bootloader design. This approach simplifies the overall implementation by avoiding the size constraints of the MBR and eliminating the need to create a dedicated stage 1 loader for each supported file system. 
1. When creating a FAT12 disk image, it is important to note that it does not initially contain the stage 1 bootloader code in its boot sector. To address this, CHB includes utilities specifically designed for FAT12 file systems that automate the process of copying the stage 1 code into the final FAT12 image. These tools preserve the BIOS Parameter Block (BPB), which is later used for proper file system access during stage 2 execution. 
8. **Building CHB** 

   Please note that there are 2 possible files that build CHB from source: using Makefile or build.sh script. 

   **IMPORTANT NOTE!:** Never run the Makefile directly because it does not contain the initial configuration and cross- compiling interface that the ./build.sh script provides. 

   Notes of build.sh script: 

- The ./build.sh script doesn't not check the version of GCC and binutils, I don't know what older versions are like, If someone wants to make a port for previous versions of GCC, I encourage them to do so. 
- It also notes that the *./build.sh* script detects whether the specified cross-compiler is installed, which means that you must have the compiler installed before enabling cross-compilation. Run ‘*./build.sh –help’* for more info. 

The *./build.sh* program has two modes: ***clean*** mode and ***build*** mode. Clean mode is run with the following command: 

*./build.sh –-clean* Or: 

*./build.sh -c* 

Note**:** Running clean mode has no effect if CHB has not been built yet. 

For compile CHB use this for build from source: 

*./build.sh –-build* 

Or: 

*./build.sh -b* 

After a successful build, all CHB binaries will be located in the *build/* directory in the project root. 

9. **Installing CHB in a FAT12 formatted image** 

   For installing CHB in a formatted FAT12 image is nessesary create empty floppy via *./mkfloppy* command.  

./mkfloppy <number\_of\_sectors> <output\_image> 

**IMPORTANT INFO:** To set the number of sectors on the floppy disk, it is necessary to keep in mind that the program only accepts the values reported by INT13,8, see *./mkfloppy* for more information. 

Then the previously created file is formatted. (in this case I use *mkfs.fat -F12*) 

**IMPORTANT INFO:** For FAT12 images is nessesary set the number to reserved sectors after MBR because loader is stored here (stage2) the program. CHB requests at least 35 sectors for store loader. See *mkfs.fat –-help* for more info 

For install CHB bootsector in the previous file run: 

*mkbs12* 

Finally, you need to run the command *./install12* to install the loader (stage2) after the MBR. 

And that's it, you now have your FAT12 image with CHB inside. 

10. **Kernel loading process** 

In an image formatted with CHB, what the loader *(stage2)* does               is search for */kernel.BIN.* 

**IMPORTANT INFO:**  

- *The kernel file must be linked at address 0x100000 (1 MB) and must be less than 128 sectors in size (due to INT13,2 limitations).*  
- *At the moment, CHB only supports binary files (not executable files) to load the kernel into memory.* 
- *CHB does’nt report number of disk, memory map, CPUID, sse, etc, So you must implement these functions in your kernel.* 
- *Your kernel MUST have compiled for 32-bit platform (iX86 or using -m32 compiler flag) CHB doesn't support long* 

  *mode!* 

11. **Filesystem and disk important notes** 
- *To load a file into the CHB memory, it currently only supports floppy disks.* 
- *CHB does not currently support booting from MBR partitions.* 
- *CHB does not support loading sectors using INT 13.4X, i.e. it does not support cdrom disks.* 
12. **CHB memory map** 

**NOTICE:** *CHB does not use the INT12 or INT15,E820 interrupts so this map is approximate to what CHB uses in memory.* 

0x0 to 0x499 – Real mode reserved. 

0x500 to 0x7AFF – Filesystem driver data. 0X7B00 to 0x7BFF – Unused memory. 

0x7C00 to 0X7FFF – Stage1 and stack. 

0x8000 to 0x7FFFF – Loader program. 

0x80000 to 0Xfffff – Upper memory. (**RESERVED**) 

13. **Have fun!** 

That's it! Once CHB is installed on the formatted image, you can start experimenting with your kernels. For now, CHB is a small bootloader, but there are plans to add new features in the future. 

14. **Final considerations** 

If you are interested in contributing to the code, the following rules must be followed: 

- All code must be released under the MIT or compatible license. 
- All changes to the code MUST be properly documented. 

  There is currently no CONTRIBUTING file, so these are the only two rules that MUST be followed. 

  At the beginning of each code file, you should find something like this: 

  */\** 

* *<file> -- <file\_description>* 

*\** 

* *Copyright (C) <year> <author>* 

*\** 

* *This file is distributed under the terms of the <license> \*/* 
15. **CHB PLAN (TODO LIST)** 
- *Add support for other filesystems.* 
- *Disk and CDROM support. (INT13,4X)* 
- *Write support. (fat12)* 
- *Executable formats. (pe, elf, coff, a.out, ...)* 
- *Support for UEFI systems.* 
- *Non x86 support.* 
- *Multiboot specification.* 
- *Long mode support.* 
- *VFS support.* 
- *Memory map.* 
- *Rework CHB error system.* 
- *Rework floppy controller in stage1.* 
- *Report disk geometry in stage1. (INT13,8)* 
- Create more complete documentation. 
16. **Changelog** 

Compared to the previous version *(v.0.02)* the following changes were made: 

- *Added simple libc. (stdlib, stdio, ctype and string)* 
- *Added C part of loader. (stage2)* 
- *Improved documentation.* 
- *FAT12 read support.* 
- *Simple INT13 floppy controller.* 
- *Simple video controller. (for 80x25 modes)* 
- *Rework of build system* 
- *Reorganization of source files.* 
17. **That’s all!** 

Now you are ready to use CHB and load your kernels. 

At the moment, CHB doesn't support keyboard input or have a simple shell that allows for further extensions to its functionality. All CHB does is automatically load the kernel without any initial configuration. 

If you encounter any issues with the code, please feel free to report them via GitHub. Have fun! 

**Andres26** 

**14-04-2025 github.com/Andres2626/CHB-Bootloader** 
