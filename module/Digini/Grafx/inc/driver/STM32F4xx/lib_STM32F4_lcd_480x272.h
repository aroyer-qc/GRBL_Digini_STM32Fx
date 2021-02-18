//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_480x272.h
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
#include "stm32f4xx.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic driver configuration
// -

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              2

#define GRAFX_USE_V_SYNC                          // We use this driver hardware support V Sync

//#define GRAFX_USE_SOFT_PIXEL                    // We use this driver DMA for this function
//#define GRAFX_USE_SOFT_BOX                      // We use this driver function
//#define GRAFX_USE_SOFT_VLINE
//#define GRAFX_USE_SOFT_HLINE
#define GRAFX_USE_SOFT_DLINE
#define GRAFX_USE_SOFT_CIRCLE

//#define GRAFX_USE_SOFT_PRINT_FONT               // we use this driver to print FONT using DMA2D
//#define GRAFX_USE_SOFT_ALPHA                    // We use this uP + LCD controller has alpha acceleration
//#define GRAFX_USE_SOFT_COPY                     // We use this driver DMA for this function
//#define GRAFX_USE_SOFT_FILL                     // We use this driver DMA for this function

//-------------------------------------------------------------------------------------------------
// -
// - This LCD generic configuration
// -

// Display size
#define GRAFX_SIZE_X                                480
#define GRAFX_SIZE_Y                                272

//-------------------------------------------------------------------------------------------------
// -
// - This LCD versus processor configuration (very specific to this combination, not digini )
// -

// Horizontal configuration
#define GRAFX_HSYNC                                 40
#define GRAFX_HBP                                   2
#define GRAFX_HFP                                   2

// Vertical configuration
#define GRAFX_VSYNC                                 9
#define GRAFX_VBP                                   2
#define GRAFX_VFP                                   2

// TODO (Alain#1#): yep do something about this.... maybe a hook
// TO DO put this in XMACRO or BSP file!!!!   God dawn it !!!
#define LCD_BL_CTRL_PORT                            GPIOA
#define LCD_BL_CTRL_PIN                             GPIO_Pin_8

//-------------------------------------------------------------------------------------------------
// enum(s)
//-------------------------------------------------------------------------------------------------

enum PixelFormatTable_e
{
    PIXEL_FORMAT_ARGB8888  = LTDC_Pixelformat_ARGB8888,
    PIXEL_FORMAT_RGB888    = LTDC_Pixelformat_RGB888,
    PIXEL_FORMAT_RGB565    = LTDC_Pixelformat_RGB565,
    PIXEL_FORMAT_ARGB1555  = LTDC_Pixelformat_ARGB1555,
    PIXEL_FORMAT_ARGB4444  = LTDC_Pixelformat_ARGB4444,
    PIXEL_FORMAT_L8        = LTDC_Pixelformat_L8,
    PIXEL_FORMAT_AL44      = LTDC_Pixelformat_AL44,
    PIXEL_FORMAT_AL88      = LTDC_Pixelformat_AL88,
    PIXEL_FORMAT_L4,
    PIXEL_FORMAT_A8,
    PIXEL_FORMAT_A4,
    GRAFX_PIXEL_FORMAT_COUNT,
};

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------

const LTDC_Layer_TypeDef* LTDC_baLayer[GRAFX_NUMBER_OF_ACTIVE_LAYER] =
{
    LTDC_Layer1,
    LTDC_Layer2
};

static bool bDriverInitialize = false;

// -------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

