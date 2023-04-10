
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

/*entry point offset*/
#define ENTRY_POINT 0x0

/*BPB start offset*/
#define BPB_BEGIN 0x4

/*BPB end offset*/
#define BPB_END 0x64

/*DAP offset*/
#define DAP_OFFSET 0x65

/*size of stage2 boot.S*/
#define STAGE2_SIZE 0x67

/*localization of stage2*/
#define STAGE2_LOC 0x68

/*segment of stage2*/
#define STAGE2_SEG 0x6a

/*LBA sector of stage2*/
#define STAGE2_SEC 0x6c

/*MBR start*/
#define MBR_OFFSET_BEGIN 410
