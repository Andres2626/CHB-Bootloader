=============================================
Technical aspects and general characteristics
=============================================

- **Simple build script:** For enable support for cross-compiling, CHB
  implements the build script, this configure important variables used
  by makefile (*e.j. compiler prefix*) and runs makefile for build CHB.
  The script does not validate tool versions, please see :ref:`building_prerequisites` 
  for more information.

- **Simple and dynamic kernel loading:** This version add support for
  very simple FS driver (*FAT12*), 'stage1' pass control to 'loader' and 
  execute kernel. At the moment CHB doesn't report BIOS disk extensions, 
  cpuid, memory map, etc. So, you would have to implement this yourself 
  in your kernel. CHB at the moment only support ELF formatted executables.

- **Protected mode:** CHB changes to protected mode before 'stage1'
  program, and runs kernel in protected mode, At the moment CHB doesn't
  support long mode, if you will load kernel using CHB, you must compile
  it with '-m32' compiler option.

- **Two staged bootloader:** CHB follows a two-stage bootloader design.
  This approach simplifies the overall implementation by avoiding the
  size constraints of the MBR and eliminating the need to create a
  dedicated 'stage1' loader for each supported file system.

- **FS utils:** When creating a FAT12 disk image, it is important to
  note that it doesn't initially contain the 'stage1' bootloader code in
  its boot sector. To address this, CHB includes utilities specifically
  designed for FAT12 file systems that automate the process of copying
  the 'stage1' code into the final FAT12 image. These tools preserve
  the BPB, which is later used for proper file system access during
  'loader' execution.

- **Memory:** CHB obtains the memory map using several BIOS calls such 
  as INT12, INT15,88 and INT15,E820, which are passed to the kernel 
  before it is executed for later use.

- **ELF Loader:** For load kernels CHB use ELF executable format, at the
  moment this loader is very tiny implementation, but sufficient for
  load and execute your kernel in memory.
