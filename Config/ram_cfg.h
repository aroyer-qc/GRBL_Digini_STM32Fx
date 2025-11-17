//-------------------------------------------------------------------------------------------------
//
//  File :  ram_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2025 Alain Royer.
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

// SDRAM Timing (Value 1 to 16)
#define CFG_SDRAM_LOAD_TO_ACTIVITY_DELAY        2   // 2 Clock cycles 								(TMRD) Delay between a 'load mode register' command and an 'active' or 'refresh' command
#define CFG_SDRAM_EXIT_SELF_REFRESH_DELAY       7   // min = 70 nSec (7 x 11.11 nSec)				(TXSR) Delay from releasing the 'self-refresh' command to issuing the 'activate' command
#define CFG_SDRAM_SELF_REFRESH_TIME             4   // min = 42 nSec (4 x 11.11 nSec) max=120k(ns)	(TRAS) Minimum self-refresh period
#define CFG_SDRAM_ROW_CYCLE_DELAY               7   // min = 63 nSec (6 x 11.11 nSec)               (TRC)  Delay between the 'refresh' command and the Activate command, as well as the delay between two consecutive 'refresh' command
#define CFG_SDRAM_WRITE_RECOVERY_TIME           2   // 2 Clock cycles								(TWR)  Delay between a 'write' and a 'pre-charge' command
#define CFG_SDRAM_RP_DELAY                      3   // min = 15 nSec (3 x 11.11 nSec)  				(TRP)  Delay between a pre-charge command and another command
#define CFG_SDRAM_RCD_DELAY                     3   // min = 15 nSec (3 x 11.11 nSec)               (TRCD) Delay between the 'activate' command and a Read/Write command

#define CFG_SDRAM_AUTO_REFRESH_CYCLE            8   // Auto refresh cycle

#define CFG_SDRAM_MRD_WRITE_BURST_MODE          FMC_SDRAM_MRD_WRITE_BURST_MODE_SINGLE
#define CFG_SDRAM_MRD_OPERATION_MODE            FMC_SDRAM_MRD_OPERATION_MODE_STANDARD
#define CFG_SDRAM_MRD_CAS_LATENCY               FMC_SDRAM_MRD_CAS_LATENCY_2
#define CFG_SDRAM_MRD_BURST_TYPE                FMC_SDRAM_MRD_BURST_TYPE_SEQUENTIAL
#define CFG_SDRAM_MRD_BURST_LENGTH              FMC_SDRAM_MRD_BURST_LENGTH_1

// SDRAM access interface configuration
#define CFG_SDRAM_BANK                          FMC_SDRAM_BANK1
#define CFG_SDRAM_COLUMN_BITS_NUMBER            FMC_SDRAM_COLUMN_BITS_NUM_8
#define CFG_SDRAM_ROW_BITS_NUMBER               FMC_SDRAM_ROW_BITS_NUM_12
#define CFG_SDRAM_MEMORY_DATA_WIDTH             FMC_SDRAM_MEM_BUS_WIDTH_16
#define CFG_SDRAM_INTERNAL_BANK_NUMBER          FMC_SDRAM_INTERN_BANKS_NUM_4
#define CFG_SDRAM_CAS_LATENCY                   FMC_SDRAM_CAS_LATENCY_2
#define CFG_SDRAM_WRITE_PROTECTION              FMC_SDRAM_WRITE_PROTECTION_DISABLE
#define CFG_SDRAM_SD_CLOCK_PERIOD               FMC_SDRAM_CLOCK_PERIOD_2
#define CFG_SDRAM_READ_BURST                    FMC_SDRAM_RBURST_ENABLE
#define CFG_SDRAM_PIPE_DELAY                    FMC_SDRAM_RPIPE_DELAY_0

#define CFG_SDRAM_REFRESH_COUNT                 1539                                // ~64 mSec Refresh at 100 Mhz (This value was found in the STM32F746 Example.)

//-------------------------------------------------------------------------------------------------
