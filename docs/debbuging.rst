Debugging CHB
=============

Currently, CHB does not offer a complete debug interface; it is best to use QEMU or BOCHS if 
necessary, among other external tools such as GDB.


Real Hardware Testing
---------------------

CHB has not been tested on physical hardware. Due to its minimal design
and limited feature set, it doesn't implement many of the basic
functions typically required by modern systems. However, no special
requirements are needed to attempt booting CHB on real hardware.

If you are interested in testing CHB on a physical machine, doing so is
encouraged, as it may help uncover bugs and improve code reliability
through real hardware testing.

**NOTE:** We are not responsible for the use of the CHB project on real
hardware. This project was originally created to be emulated in QEMU or
BOCHS, although it can be ported to real hardware. Please be aware of
potential BIOS, motherboard, and code bugs that may or may not have been
discovered.

However, if your intention is to run CHB on real hardware, it is
necessary to keep in mind that at the moment it only supports floppy as
the primary storage medium.
