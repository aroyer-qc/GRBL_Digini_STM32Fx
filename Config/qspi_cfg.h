//-------------------------------------------------------------------------------------------------
//
//  File :  qspi_cfg.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define QSPI_SUPPORT_DUAL_FLASH_MODE                    DEF_DISABLED                   // Not tested (future use)
#define QSPI_SUPPORT_DOUBLE_DATA_RATE                   DEF_DISABLED
#define QSPI_SUPPORT_FLASH_RESET_PIN                    DEF_DISABLED
#define QSPI_MODE_MEMORY_MAPPED                         DEF_DISABLED                   // In a file system usage,this cannot be used as memory mapped mode
#define QSPI_READ_DISCOVERY_PARAMETER                   DEF_ENABLED                    // Discovery parameter

#if (QSPI_READ_DISCOVERY_PARAMETER == DEF_DISABLED)
#define QSPI_DUMMY_CLOCK_CYCLES_READ_CFG                x                               // if the memory does not support discovery parameter, write here the value set by the manufacturer
#endif

#define QSPI_MEMORY_SIZE_MBYTE_CFG                      16
#define QSPI_FLASH_PAGE_SIZE                            256
#define QSPI_FLASH_SECTOR_SIZE                          4096
//#define SFLASH_TOTAL_SIZE                   0x1000000           // 16 MBytes
//#define SFLASH_READ_ID                      0x9D6019
//#define SFLASH_STATUS_DEFAULT_ACCESS_MASK   0xFC
//#define SFLASH_STATUS_DEFAULT_ACCESS        0x00
//#define SFLASH_SECTOR_ERASE_TIME            (300 + 10)
#define QSPI_FLASH_PAGE_PROGRAM_TIME                    (1 + 5)

// TODO // check if we install auto configuration for this driver
// Non common command set for the flash memory
#define QSPI_QUAD_OUT_FAST_READ_CMD_CFG                 0x6B
#define QSPI_QUAD_PAGE_PROGRAM_CMD_CFG                  0x02
#define QSPI_SECTOR_ERASE_CMD_CFG                       0x20
#define QSPI_BLOCK_ERASE_CMD_CFG                        0xD8
#define QSPI_CHIP_ERASE_CMD_CFG                         0x60

//Setting for STM32F7
#define QSPI_SPEED_CFG                                  QSPI_SPEED_HIGH
#define QSPI_ISR_PRIORITY_CFG                           6
#define QSPI_DMA_ISR_PRIORITY_CFG                       6
#define QSPI_MODE_INDIRECT_CFG                          DEF_ENABLED
#define QSPI_FIFO_THRESHOLD_CFG                         1
#define QSPI_PRESCALER_CFG                              64                              // Max 256
#define QSPI_SAMPLE_SHIFTING_CFG                        QSPI_SAMPLE_SHIFTING_HALFCYCLE
#define QSPI_FLASH_ID_CFG                               QSPI_FLASH_ID_1                 // if DUAL Enable QUADSPI_FLASH_ID_1 or QUADSPI_FLASH_ID_2
#define QSPI_CS_HIGH_TIME_CFG                           QSPI_CS_HIGH_TIME_3_CYCLE
#define QSPI_CLOCK_MODE_CFG                             QSPI_CLOCK_MODE_3
#define QSPI_AUTOMATIC_POLLING_INTERVAL_CFG             30

//-------------------------------------------------------------------------------------------------
