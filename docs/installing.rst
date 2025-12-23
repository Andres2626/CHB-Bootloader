Installing CHB
==============

Currently, CHB only supports floppy disk images but can support HDDs, but 
note that the only available file system is FAT12. (TODO: implement this)

Creating empty floppy image
---------------------------

CHB offers a utility for creating blank floppy disk images. **All**
images generated with ‘\ *mkfloppy*\ ’ have the exact number of sectors
required to comply with INT13.8 on IBM/PC machines. This is because QEMU
or BOCHS will fail to read the floppy disk if it doesn't have a specific
number of sectors, and it's also necessary for mkfs.fat to recognize it
as a valid image.

Please note that 'stage1' loads 'loader' by locating after the MBR.
'stage1' does not function as a locator within the file system; that is
the responsibility of 'loader'.

**MKFS.FAT IMPORTANT NOTE**: If you create an image with mkfs.fat (or
any FAT12 utility) it is important to reserve a number of sectors
greater than or equal to 45 so that 'loader' can be stored there and
'stage1' can find it.

For create an empty floppy image is necessary invoke *‘mkfloppy’* CHB
command.

After invoking the command, it should create an empty image with the
specified number of sectors.

**CAUTION:** *The program does not detect whether the image is empty or
not, so it rewrites all the information contained in it.*

Done! Now you have an empty image ready to install and run CHB within
it.

Installing ‘stage1’ into empty floppy image
-------------------------------------------

Once an empty floppy image has been created, the command provided by CHB
for 'mkfs12' is invoked; this is used to **copy the code** from 'stage1'
while preserving the information from the sector that generated
'mkfs.fat'.

Installing ‘loader’ into floppy image
-------------------------------------

Finally, for install loader after MBR invoke ‘install12’ CHB command.

That’s it you have an CHB floppy image ready to be run in your virtual
machine.

NOTE OF HOST BINARIES
---------------------

Because the binaries for all utilities are not available in the main
repository, you will need to compile some of these utilities yourself
and adjust the *\`--prefix\`* option. Then, use the target binaries
provided in the repository and move them to the *\`prefix\`* directory.
