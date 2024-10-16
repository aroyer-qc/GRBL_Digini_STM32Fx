//-------------------------------------------------------------------------------------------------
//
//  File :  i2c_cfg.h
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
//  Notes: I2C configuration for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------
// Global Define(s)
//-------------------------------------------------------------------------------------------------

#define I2C_DRIVER_SUPPORT_I2C1_CFG             DEF_ENABLED
#define I2C_DRIVER_SUPPORT_I2C2_CFG             DEF_DISABLED
#define I2C_DRIVER_SUPPORT_I2C3_CFG             DEF_ENABLED
#define I2C_DRIVER_SUPPORT_I2C4_CFG             DEF_DISABLED


// If set to DEF_DISABLED, Addressing mode will be 7 bits. no dual address mode, no slave support
#define I2C_DRIVER_SUPPORT_ADVANCED_MODE_CFG    DEF_DISABLED

#define I2C_DRIVER_SUPPORT_CLK_SELECTION_CFG    DEF_DISABLED

//-------------------------------------------------------------------------------------------------

