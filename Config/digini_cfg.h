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
//  Define(s)
//-------------------------------------------------------------------------------------------------

//#define ETH_SOCK                                        0
//#define ETH_PORT                                        30501

#define DIGINI_USE_ASSERT                               DEF_DISABLED

#define DIGINI_RESTRICT_MEMORY_ALLOC_TO_BLOCK_SIZE      DEF_DISABLED    // LIB_Memory.x. If Enable we allow only closest block size that are greater to be allocated

//#define DIGINI_APP_USE_TICK_HOOK                      DEF_ENABLED

#define DIGINI_USE_CONSOLE                              DEF_ENABLED
#define DIGINI_USE_CMD_LINE                             DEF_ENABLED
#define DIGINI_USE_VT100_MENU                           DEF_ENABLED

#define DIGINI_USE_ETHERNET                             DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// High level Peripheral
//
//#define DIGINI_USE_SPI_DEVICE
#define DIGINI_SD_CARD_DETECT_IO                        DETECT_SD_CARD

//-------------------------------------------------------------------------------------------------
// EEPROM support
//

#define DIGINI_USE_EEPROM			                    DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Multi language support
//
// Note: If it is DEF_ENABLED then you need to add to a database the item DIGINI_SYSTEM_LANGUAGE

#define DIGINI_USE_MULTI_LANGUAGE_SUPPORT               DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// FAT FS Peripheral support
//

#define DIGINI_USE_FATFS                                DEF_ENABLED

#define DIGINI_FATFS_USE_SPI_FLASH_CHIP                 DEF_DISABLED  // to be added to peripheral list
#define DIGINI_FATFS_USE_QSPI                           DEF_DISABLED  // to be added to peripheral list
#define DIGINI_FATFS_USE_SDIO_SD_CARD                   DEF_ENABLED
#define DIGINI_FATFS_USE_SPI_SD_CARD                    DEF_DISABLED
#define DIGINI_FATFS_USE_USB_KEY                        DEF_DISABLED
#define DIGINI_FATFS_USE_RAM_DISK                       DEF_DISABLED

//-------------------------------------------------------------------------------------------------
// GRAFX Configuration
//

#define DIGINI_USE_GRAFX                                DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// CRC Configuration
//

#define DIGINI_USE_CRC                                  DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//
// Note: If it is DEF_ENABLED then you need also need to  "NOS_CONFIG_DEBUG  1"    in nOSConfig.h

#define DIGINI_USE_STACKTISTIC                          DEF_ENABLED                 // Allow debugging of the stack, per code or using VT100 terminal
#define DIGINI_STACKTISTIC_NUMBER_OF_STACK              10
#define DIGINI_STACKTISTIC_WATER_MARK_CODE              0xFFFFFFFFUL                // This is for nOS. Depend on the value put in stack by OS. FreeRTOS it's 0xA5

//-------------------------------------------------------------------------------------------------
// Debug special feature

#define DIGINI_DEBUG_SDCARD_INFO_ON_VT100               DEF_ENABLED


//-------------------------------------------------------------------------------------------------
