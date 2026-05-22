//-------------------------------------------------------------------------------------------------
//
//  File :  fmc_lcd_cfg.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2026 Alain Royer.
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

//need to be adapted for this board

#define CFG_FMC_LCD_BANK                                FMC_LCD_BANK1               // LCD Bank (NE1)
#define CFG_FMC_LCD_MEM_BUS_WIDTH                       FMC_LCD_MEM_BUS_WIDTH_16    // LCD Memory Bus Width

// LCD Timing
#define CFG_FMC_LCD_TIMING_ADDRESS_SETUP_TIME           4                           // ~12 nSec
#define CFG_FMC_LCD_TIMING_ADDRESS_HOLD_TIME            2                           // ~6  nSec
#define CFG_FMC_LCD_TIMING_DATA_SETUP_TIME              4                           // ~12 nSec

#define CFG_FMC_LCD_WRITE_TIMING_ADDRESS_SETUP_TIME     4                           // ~12 nSec
#define CFG_FMC_LCD_WRITE_TIMING_ADDRESS_HOLD_TIME      2                           // ~6  nSec
#define CFG_FMC_LCD_WRITE_TIMING_DATA_SETUP_TIME        6                           // ~36 nSec

//-------------------------------------------------------------------------------------------------
