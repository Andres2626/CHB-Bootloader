
/*
*	 CHB - Computers Help Bootloader
*    Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023  Andres26, Computers Help
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*Bootsector location on memory*/
#define BOOT_LOC 0x7c00

/*Bootsector signature*/
#define BOOT_SEC_SIG 0xaa55

/*size of stage1*/
#define BOOT_SEC_SIZE 512

/*stack size (stage1)*/
#define STACK_SIZE BOOT_SEC_SIZE - 2

/*magic number of bootsector*/
#define BOOT_SEC_MAGIC 0xaa55

/*Message defs.*/
#define MSG(x) mov $x, %si; call bios_print
#define ERR(x) MSG(x); MSG(error)