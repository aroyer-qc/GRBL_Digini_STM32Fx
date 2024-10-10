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

#define DIGINI_MAX_PRINT_SIZE                           256


#define DIGINI_USE_ASSERT                               DEF_DISABLED

//#define DIGINI_APP_USE_TICK_HOOK                      DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Static memory allocation
//
#define DIGINI_USE_STATIC_MEMORY_ALLOC                  DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Database support
//
#define DIGINI_USE_DATABASE                             DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Static memory allocation
//
#define DIGINI_USE_LABEL                                DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// COMM support (Console, CLI, VT100)
//
#define DIGINI_USE_COMM_MODULE                          DEF_ENABLED
#define DIGINI_USE_COMM_AS_A_TASK                       DEF_DISABLED         // Otherwise it is a process that need to be incluse in your choice of task

// Only set this if DIGINI_USE_COMM_AS_A_TASK is DEF_ENABLED
#define TASK_COMM_PRIO                                  7
#define TASK_COMM_STACK_SIZE                            256

//-------------------------------------------------------------------------------------------------
// COMM support (Console, CLI, VT100)
//
#define DIGINI_USE_COMM_MODULE                          DEF_ENABLED

#define DIGINI_USE_CONSOLE                              DEF_ENABLED
#define DIGINI_USE_CMD_LINE                             DEF_ENABLED
#define DIGINI_USE_VT100_MENU                           DEF_ENABLED

#define DIGINI_USE_DEBUG_IN_CONSOLE                     DEF_ENABLED
#define DIGINI_USE_HELP_IN_CONSOLE                      DEF_ENABLED     // Adding the help command to the command line

#define DIGINI_USE_COMM_AS_A_TASK                       DEF_DISABLED    // Otherwise it is a process that need to be include in your choice of task
#define TASK_COMM_PRIO                                  7
#define TASK_COMM_STACK_SIZE                            256

//-------------------------------------------------------------------------------------------------
// Ethernet
//
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
// Software CRC Configuration
//
#define DIGINI_USE_CRC                                  DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// MPU Configuration
//
#define DIGINI_MPU_DRIVER                               DEF_DISABLED

//-------------------------------------------------------------------------------------------------
//
// Note: If it is DEF_ENABLED then you need also need to  "NOS_CONFIG_DEBUG  1"    in nOSConfig.h
#define DIGINI_USE_STACKTISTIC                          DEF_ENABLED                 // Allow debugging of the stack, per code or using VT100 terminal
#define DIGINI_STACKTISTIC_NUMBER_OF_STACK              10

//-------------------------------------------------------------------------------------------------
// Debug special feature
#define DIGINI_DEBUG_SDCARD_INFO_ON_VT100               DEF_ENABLED

//-------------------------------------------------------------------------------------------------
