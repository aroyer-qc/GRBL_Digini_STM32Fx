//-------------------------------------------------------------------------------------------------
//
//  File : FatFs_var.h
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
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// constf(s)
//-------------------------------------------------------------------------------------------------

#ifdef DISKIO_GLOBAL

//SPI_Param_t SPI_FlashParameter               // Parameter for initialization of the SPI for the flash
//{
//    &SPI_ForFlash,
//    FLASH_AUTO_DETECT,
//    IO_CS_FLASH,
//};
#else

//extern SPI_Param_t SPI_FlashParameter;

#endif

//-------------------------------------------------------------------------------------------------
// forward declaration(s)
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
class FatFS_SPI_Memory;
class FatFS_USB_Key;
#endif

//-------------------------------------------------------------------------------------------------
// X-Macro(s)
//-------------------------------------------------------------------------------------------------

#define FAT_FS_DRIVE_DEF(X_DRIVE)\
/*  		 ID of Disk,        Specific FatFs class, Object to create,    Parameter for     */          \
    X_DRIVE( DISK_SPI_FLASH,   	FatFS_SPI_Memory,     SPI_FlashDisk,        (void*)&SPI_FlashParameter)	 \
    X_DRIVE( DISK_USB_KEY,      FatFS_USB_Key,        USB_KeyDisk,          nullptr )		             \

//-------------------------------------------------------------------------------------------------
