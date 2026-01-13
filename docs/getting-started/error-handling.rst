==============
Error handling
==============

CHB has several ways of reporting errors that occur at all its stages;
for 'stage1' it simply prints an error message and dies, and in 'loader'
the system is based on error numbers.

‘stage1’ possible errors
------------------------

- **Disk Read error**: It appears if the disk could not be read to
  locate 'loader' and prints **'READ Error.**'

- **Disk geometry error**: The disk geometry has an invalid parameter,
  prints '**GEOM Error**'
  
- **Hard disk error**: 'stage1' determines that the disk is an HDD and 
   is not working, prints '**HDD Error**'

- **Floppy error**: The determination of the floppy's geometry was 
  unsuccessful. prints '**FLOPPY Error**'

**IMPORTANT NOTE:** ‘\ *stage1*\ ’ has a mechanism to hand control
back to the BIOS via INT18 in case of a failure. If this mechanism is
not present, the program enters an infinite loop. The *hlt*
instruction is NEVER called because in some BIOSes and firmware,
executing the *cli* leaves the CPU in a dead state, preventing any
further action in case of failure (*Ctrl+Alt+Del*) and rendering the
CPU unusable for restarting.

‘loader’ possible errors
------------------------

Below is the list of errors reported by the 'loader'

- **CODE 1 (ENOMEM)**: This occurs when the FAT controller detects that
  the amount of memory reserved for storing FS information has been
  exceeded.

- **CODE 2 (EMFILE)**: This occurs when the maximum number of open files
  authorized by the VFS is exceeded.

- **CODE 3 (EKERN):** The kernel was not found on the disk.

- **CODE 4 (EIFS):** If any attribute of the FAT or VFS file system is
  invalid, this can happen if the reported sector size is invalid, or if
  the open file has an invalid attribute.

- **CODE 5 (EREGFS)**: The maximum number of registered file systems was
  exceeded.

- **CODE 6 (EIO):** A disk error occurred, either reading sectors or
  obtaining attributes.

- **CODE 7 (ENOEXEC)**: The executable format is invalid.

- **CODE 8 (EINVAL)**: It is equivalent to an invalid attribute.

- **CODE 9 (EFAULT)**: Some structure contains or is a null pointer.

- **CODE 10 (ESYSINIT):** Some CHB subsystem failed to start.

- **CODE 11 (EFILE)**: The file to be opened was not found in the FS.

- **CODE 12 (EKERNLOC)**: Kernel is located before 1MB.

- **CODE 12 (EFS)**: Generic FS error.

- **CODE 254 (ENOIMP)**: Function not implemented.

- **CODE 255 (EUNKNOUN)**: Unknown error.

When an error occurs in the loader, the error is displayed as
follows:

.. code-block:: bash 
 
    CHB error <code>: <message>

After the message is printed, CHB usually prints a panic message and 
shuts down the entire system. This call to `panic()` enters an infinite 
loop until a key is pressed; specifically, `loader/init.C` defines the 
`panic()` function as follows:

.. code-block:: C
 
    /* 
     * panic(): The `panic()` function of `CHB` is used when recovery from 
     * an error is impossible. It typically follows the error code generated 
     * by the function that halted the program. Think of it as a simplified 
     * version of the `panic()` function in Unix. This function prints the 
     * panic message and never returns; it ultimately restarts the machine 
     * if a key is pressed.
     */