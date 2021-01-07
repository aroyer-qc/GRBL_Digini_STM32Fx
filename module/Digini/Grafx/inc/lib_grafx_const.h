//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_const.h
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

#include "lib_grafx.h"
#include "color_cfg.h"

//-------------------------------------------------------------------------------------------------
// Const(s)
//-------------------------------------------------------------------------------------------------

extern const int8_t GFX_PixelSize[PIXEL_FORMAT_COUNT];
extern const s32_t  GFX_ColorTable[COL_NB_COLOR_CONST];

#ifdef GFX_GLOBAL

extern const int8_t GFX_PixelSize[PIXEL_FORMAT_COUNT] =
{
  #ifdef GRAFX_COLOR_ARGB8888
    4,
  #endif
  #ifdef GRAFX_COLOR_RGB888
    3,
  #endif
  #ifdef GRAFX_COLOR_RGB565
    2,
  #endif
  #ifdef GRAFX_COLOR_ARGB1555
    2,
  #endif
  #ifdef GRAFX_COLOR_ARGB4444
    2,
  #endif
  #ifdef GRAFX_COLOR_L8
    1,
  #endif
  #ifdef GRAFX_COLOR_AL44
    1,
  #endif
  #ifdef GRAFX_COLOR_AL88
    1,
  #endif
  #ifdef GRAFX_COLOR_L4
    1,
  #endif
  #ifdef GRAFX_COLOR_A8
    1,
  #endif
  #ifdef GRAFX_COLOR_A4
    1,
  #endif
  #ifdef GRAFX_COLOR_RGB332
    1,
  #endif
  #ifdef GRAFX_COLOR_RGB444
    2,
  #endif
    -1
};

extern const s32_t GFX_ColorTable[COL_NB_COLOR_CONST] =
{
  #define X_COLOR(ENUM_ID, VALUE) {VALUE},
    COLOR_DEF
  #undef X_COLOR
  #define X_GRAY(ENUM_ID, VALUE) {VALUE},
    GRAY_DEF
  #undef X_GRAY

  #ifdef CUSTOM_COLOR_DEF
   #define X_CUSTOM_COLOR(ENUM_ID, VALUE) {VALUE},
    CUSTOM_COLOR_DEF
   #undef  X_CUSTOM_COLOR
  #endif
};

#endif

//-------------------------------------------------------------------------------------------------


