//
//  File : lib_template_lcd_driver.h
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
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic driver configuration
// -

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              1 - x                 // How many layer the LCD or virtual screen support
#define GRAFX_PIXEL_FORMAT_COUNT                  11                    // how many colour format are supported

#define GRAFX_USE_V_SYNC                          // We use this driver hardware support V Sync

//#define GRAFX_USE_SOFT_PIXEL                    // if commented, we use this driver DMA for this function
//#define GRAFX_USE_SOFT_BOX                      // if commented, we use this driver function to draw a box (rectangle)
//#define GRAFX_USE_SOFT_VLINE                    // if commented, we use this driver function to draw a vertical line
//#define GRAFX_USE_SOFT_HLINE                    // if commented, we use this driver function to draw a horizontal line
#define GRAFX_USE_SOFT_DLINE                      // if commented, we use this driver function to draw a diagonal line
#define GRAFX_USE_SOFT_CIRCLE                     // if commented, we use this driver function to draw a circle line

//#define GRAFX_USE_SOFT_PRINT_FONT               // we use this driver to print FONT using DMA2D
//#define GRAFX_USE_SOFT_ALPHA                    // We use this uP + LCD controller has alpha acceleration
//#define GRAFX_USE_SOFT_COPY                     // We use this driver DMA for this function
//#define GRAFX_USE_SOFT_FILL                     // We use this driver DMA for this function

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic configuration
// -

// Display size
#define GRAFX_SIZE_X                                480 // example
#define GRAFX_SIZE_Y                                272 // example

//-------------------------------------------------------------------------------------------------
// -
// - This LCD versus processor configuration (very specific to this combination, not digini )
// -

// Put here driver specific to the LCD and processor combination

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------


static bool bDriverInitialize = false;

// -------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

