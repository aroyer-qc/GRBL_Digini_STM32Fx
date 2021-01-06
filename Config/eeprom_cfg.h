//-------------------------------------------------------------------------------------------------
//
//  File :  eeprom_cfg.h
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
// Note: EEprom configuration for GRBL
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

// E2_M24LC32A
#define E2_M24LC32A_SIZE            4096
#define E2_M24LC32A_NB_OF_PAGE      32

// E2_2464
#define E2_2464_SIZE                8192
#define E2_2464_NB_OF_PAGE          32

// E2_24128
#define E2_24128_SIZE               16384
#define E2_24128_NB_OF_PAGE         64

// E2_24256
#define E2_24256_SIZE               32768
#define E2_24256_NB_OF_PAGE         64

// E2_24512
#define E2_24512_SIZE               65536
#define E2_24512_NB_OF_PAGE         128

// E2_M24M01
#define E2_M24M01_SIZE              131072
#define E2_M24M01_NB_OF_PAGE        256

//-------------------------------------------------------------------------------------------------

// User application EEprom config
#define GRBL_EEPROM_SIZE            E2_24256_SIZE
#define GRBL_NB_OF_PAGE             E2_24256_NB_OF_PAGE
#define GRBL_DEVICE_ADDRESS         0x50

//-------------------------------------------------------------------------------------------------

