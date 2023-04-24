//-------------------------------------------------------------------------------------------------
//
//  File : digini_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2020 Alain Royer.
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
//
// NOTE(S) For Graphic related configuration, see grafx_cfg.h
//
//-------------------------------------------------------------------------------------------------


#pragma once

//-------------------------------------------------------------------------------------------------
// #define DIGINI_USE_ASSERT// Convert to DEF_ENABLED/DEF_DISABLED
#define DIGINI_USE_CONSOLE                      DEF_DISABLED
#define DIGINI_USE_CMD_LINE                     DEF_DISABLED
#define DIGINI_USE_VT100_MENU                   DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// High level Peripheral
//
//#define DIGINI_USE_SPI_DEVICE
#define DIGINI_SD_CARD_DETECT_IO                DETECT_SD_CARD

//-------------------------------------------------------------------------------------------------
// EEPROM support
//

#define DIGINI_USE_EEPROM			// Convert to DEF_ENABLED/DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// Multi language support
//
// Note: If it is DEF_ENABLED then you need to add to a database the item DIGINI_SYSTEM_LANGUAGE

#define DIGINI_USE_MULTI_LANGUAGE_SUPPORT       DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// FAT FS Peripheral support
//

#define DIGINI_USE_FATFS

#define DIGINI_FATFS_USE_SPI_FLASH_CHIP         DEF_DISABLED  // to be added to peripheral list
#define DIGINI_FATFS_USE_QSPI                   DEF_DISABLED  // to be added to peripheral list
#define DIGINI_FATFS_USE_SDIO_SD_CARD           DEF_ENABLED
#define DIGINI_FATFS_USE_SPI_SD_CARD            DEF_DISABLED
#define DIGINI_FATFS_USE_USB_KEY                DEF_DISABLED
#define DIGINI_FATFS_USE_RAM_DISK               DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// GRAFX Configuration
//

#define DIGINI_USE_GRAFX

//-------------------------------------------------------------------------------------------------
