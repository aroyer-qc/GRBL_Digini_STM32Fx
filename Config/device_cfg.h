//-------------------------------------------------------------------------------------------------
//
//  File :  device_cfg.h
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


// Example!!!!

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

// Config for lib_LMT86
//#define USE_LMT86_LOOK_UP_TABLE
//#define USE_LMT86_FLOAT_CALCULATION

// Config for lib_class_audio_codec_C43L22
//#define CS43L22_VERIFY_WRITTEN_DATA
//#define CS43L22_CODEC_STANDARD                  CS43L22_STANDARD_PHILLIPS
//#define CS43L22_OUTPUT_DEVICE                   CS43L22_OUTPUT_DEVICE_HEADPHONE
//#define CS43L22_DEFAULT_VOLUME                  0

//#define DS3502_VOLUME_I2C_SLAVE_ADDRESS         0x28

#define MCP23008_INPUT_1_8_IOEXP_I2C_SLAVE_ADDRESS  0x20
#define MCP23008_INPUT_8_16_IOEXP_I2C_SLAVE_ADDRESS 0x21
#define MCP23017_IO_EXPANDER_I2C_SLAVE_ADDRESS      0x24

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

// Put here included for all high level driver. Driver for device not in the CPU
//#include "lib_class_quadrature_encoder.h"
//#include "lib_class_I2C_si4703.h"
#include "lib_class_I2C_EEprom.h"
#include "lib_class_pulse_counter.h"
#include "lib_class_I2C_MCP230xx.h"
//#include "lib_class_I2C_DS3502.h"
//#include "lib_class_audio_codec_CS43L22.h"
//#include "lib_class_gpio_MAX4598.h"
//#include "lib_LMT86.h"
//#include "lib_class_WS281x.h"

//-------------------------------------------------------------------------------------------------
