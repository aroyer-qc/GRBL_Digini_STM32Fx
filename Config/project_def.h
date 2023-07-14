//-------------------------------------------------------------------------------------------------
//
//  File : project_def.h
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
//  Notes : This file hold all define for the application setting
//          For hardware setting see: 'bsp.h'
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
//  Include(s)
//-------------------------------------------------------------------------------------------------

#include "grbl_advance.h"

//-------------------------------------------------------------------------------------------------

extern const char OUR_VENDOR_NAME[8];
extern const char OUR_MODEL_NAME[18];
extern const char OUR_FIRMWARE_NAME[17];
extern const char OUR_FIRMWARE_VERSION[9];
extern const char OUR_FIRMWARE_GUI_NAME[17];
extern const char OUR_FIRMWARE_GUI_VERSION[17];
extern const char OUR_SERIAL_NUMBER[10];
extern const char OUR_BUILD_DATE[25];

#ifdef BSP_GLOBAL
const char OUR_VENDOR_NAME[]          = { "Digini\r"                 };
const char OUR_MODEL_NAME[]           = { "STM32F746G-DISCO\r"       };
const char OUR_FIRMWARE_NAME[]        = { "GRBL"                     };
const char OUR_FIRMWARE_VERSION[]     = { GRBL_VERSION "\r"          };
const char OUR_FIRMWARE_GUI_NAME[]    = { "Digini GUI\r"             };
const char OUR_FIRMWARE_GUI_VERSION[] = { "V2.01\r"                  };
const char OUR_SERIAL_NUMBER[]        = { "00000000\r"               };
const char OUR_BUILD_DATE[]           = { __DATE__ "  " __TIME__ "\r"};
#endif

//-------------------------------------------------------------------------------------------------
