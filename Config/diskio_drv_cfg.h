//-------------------------------------------------------------------------------------------------
//
//  File : diskio_drv_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
// Email: aroyer.qc@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
// AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//
//  Add here any custom driver for the FatFS. 
//
//
//-------------------------------------------------------------------------------------------------

// #include "lib_class_fatfs_my_custom_driver.h"
// #include etc..

#if 0   // Example of custom driver (as anything is possible)

#define FAT_FS_CUSTOM_DRIVE_DEF(X_DRIVE)\
/*  Will create custom drive */\
    X_DRIVE( DISK_EEPROM,    FatFS_Eeprom,     I2C_Eeprom,  &myI2C_Eeprom))\
    X_DRIVE( DISK_ROM,       FatFS_ROM_memory, ROM_Memory,  &myROM_Memory))\
    X_DRIVE( DISK_ETEHERNET, FatFS_Ethernet,   ETH_File,    &myEth_File  ))\

#endif

//-------------------------------------------------------------------------------------------------
