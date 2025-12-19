VFS
===

The CHB virtual file system (*CVFS*) operates very simply, although it
uses complex data structures; it's nothing like something more complex
such as the Linux file system or GRUB. Therefore, this implementation is
sufficient for handling multiple files and filesystems, making it easier
to load into the kernel.

Registered FS concept
---------------------

In order for the FS to perform all the necessary operations with the
files, each FS is registered with its auxiliary functions, thus
preventing the loader from handling the FS directly; instead, the VFS
handles each open file by communicating with the FS.

Path prefix
-----------

To locate a file in a particular file system, the VFS does the
following:

- For all mounted filesystem it is organized in the order in which a
  letter of the alphabet is assembled. **NOTE:** This letter is not
  case-sensitive

- The letter is placed at the beginning of the path of the file you want
  to search for, like this: *'/a/path/to/file.txt'*

- When a file is opened and read, the VFS separates the path from the
  prefix and uses the particular FS driver to open the file.

Inode
-----

Each time a file is opened, an open file number is assigned, this is the
*'inode_number'* and the **inode** contains useful information about the
file and the pointer to the file information obtained in the FS.

VFS Functions
-------------

Currently, the VFS only supports the following calls:

- **vfs_detect()** -- detects the fs according to the disk format.

- **vfs_mount()** -- mount a storage medium in memory.

- **vfs_unmount()** -- deletes the data from the FS mounted in memory.

- **vfs_open()** -- open a file.

- **vfs_read()** -- reads a file into memory.

Detecting FS
------------

To detect a file system (FS) on a storage medium, the disk number
provided by the BIOS is used, and then the attributes used to recognize
the FS depend on the FS driver. For example, for FAT12 it detects the
signature 0xAA55 at the end of the bootsector.
