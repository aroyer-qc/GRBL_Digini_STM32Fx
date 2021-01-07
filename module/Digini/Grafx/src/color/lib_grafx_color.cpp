//-------------------------------------------------------------------------------------------------
//
//  File : lib_color.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_typedef.h"
#include "lib_grafx.h"

//-------------------------------------------------------------------------------------------------
// Enum(s)
//-------------------------------------------------------------------------------------------------

enum __ColorChannel_e
{
    CHANNEL_BLUE = 0,
    CHANNEL_GREEN,
    CHANNEL_RED,
    CHANNEL_ALPHA,
} ColorChannel_e;

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetFormatColor
//
//   Parameter(s):  PixelFormat_e   PixelFormat
//                  eColorTable     Index
//   Return Value:  uint32_t        Color
//
//   Description:   Get the color from table and apply conversion according to format
//
//-------------------------------------------------------------------------------------------------
uint32_t GetFormatColor(PixelFormat_e PixelFormat, ColorTable_e Index)
{
    s32_t    Color;
    s32_t    NewColor;

    Color = GFX_ColorTable[Index];

    switch(PixelFormat)
    {
      #ifdef GRAFX_COLOR_ARGB4444
        case PIXEL_FORMAT_ARGB4444:
        {
            NewColor.u_16.u1      = 0;
            NewColor.u8_Array[1]  = (Color.u8_Array[CHANNEL_ALPHA] & 0xF0);                       // 256 level down to 16
            NewColor.u8_Array[1] |= (Color.u8_Array[CHANNEL_RED] >> 4);
            NewColor.u8_Array[0]  = (Color.u8_Array[CHANNEL_GREEN] & 0xF0);
            NewColor.u8_Array[0]  = (Color.u8_Array[CHANNEL_BLUE] >> 4);
            break;
        }
      #endif

      #ifdef GRAFX_COLOR_RGB565
        case PIXEL_FORMAT_RGB565:                                                                              // No alpha
        {
            NewColor.u_16.u1      = 0;
            NewColor.u8_Array[1]  = (Color.u8_Array[CHANNEL_RED] & 0xF8);                         // 256 level down to 64
            NewColor.u8_Array[1] |= (Color.u8_Array[CHANNEL_GREEN] >> 5);                         // 256 level down to 32
            NewColor.u8_Array[0]  = (Color.u8_Array[CHANNEL_GREEN] & 0x1C) << 3;
            NewColor.u8_Array[0] |= (Color.u8_Array[CHANNEL_BLUE] >> 3);                          // 256 level down to 64
            break;
        }
      #endif

      #ifdef GRAFX_COLOR_ARGB1555
        case PIXEL_FORMAT_ARGB1555:
        {
            NewColor.u_16.u1      = 0;
            NewColor.u8_Array[1]  =  (Color.u8_Array[CHANNEL_RED] == 0x80) ? 0x80 : 0x00;         // Transparency bit (threshold at 50%)
            NewColor.u8_Array[1] |= ((Color.u8_Array[CHANNEL_RED] & 0xF8) >> 1);                  // 256 level down to 32
            NewColor.u8_Array[1] |=  (Color.u8_Array[CHANNEL_GREEN] >> 6);                        // 256 level down to 32
            NewColor.u8_Array[0]  =  (Color.u8_Array[CHANNEL_GREEN] & 0x38) << 2;
            NewColor.u8_Array[0] |=  (Color.u8_Array[CHANNEL_BLUE] >> 3);                         // 256 level down to 32
            break;
        }
      #endif

      #ifdef GRAFX_COLOR_RGB888
        case PIXEL_FORMAT_RGB888:
        {
            NewColor.u_32 = Color.u_32;
            NewColor.u8_Array[CHANNEL_ALPHA] = 0;
            break;
        }
      #endif

        // TO DO
      #ifdef GRAFX_COLOR_L8
        case PIXEL_FORMAT_L8:        // Lookup table  16 color
      #endif

      #ifdef GRAFX_COLOR_AL44
        case PIXEL_FORMAT_AL44:      // Lookup table  16 alpha level,  16 color
      #endif

      #ifdef GRAFX_COLOR_AL88
        case PIXEL_FORMAT_AL88:      // Lookup table  256 alpha level, 256 color
      #endif

      #ifdef GRAFX_COLOR_ARGB8888
        case PIXEL_FORMAT_ARGB8888:  // Nothing to do
      #endif
      #ifdef GRAFX_COLOR_L4
        case PIXEL_FORMAT_L4:
      #endif
      #ifdef GRAFX_COLOR_A8
        case PIXEL_FORMAT_A8:
      #endif
      #ifdef GRAFX_COLOR_A4
        case PIXEL_FORMAT_A4:
      #endif
      #ifdef GRAFX_COLOR_RGB332
        case PIXEL_FORMAT_RGB332:
      #endif
      #ifdef GRAFX_COLOR_RGB444
        case PIXEL_FORMAT_RGB444:
      #endif

        default:
        {
            NewColor.u_32 = Color.u_32;
            break;
        }
    }

    return NewColor.u_32;
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: GetUpConvertColor
//
//   Parameter(s):  PixelFormat_e   PixelFormat
//                  uint32_t        dwColor
//   Return Value:  uint32_t        dwColor
//
//   Description:   Upconverter from any format to 32 Bit's ARGB
//
//-------------------------------------------------------------------------------------------------
uint32_t GetUpConvertColor(PixelFormat_e PixelFormat, uint32_t Color)
{
    s32_t _Color;

    switch(PixelFormat)
    {
      #ifdef GRAFX_COLOR_ARGB4444
        case PIXEL_FORMAT_ARGB4444:
        {
             _Color.u8_Array[3] = (uint8_t) (Color >> 8)           | 0x0F;
             _Color.u8_Array[2] = (uint8_t)((Color & 0x0F00) >> 4) | 0x0F;
             _Color.u8_Array[1] = (uint8_t) (Color & 0x00F0)       | 0x0F;
             _Color.u8_Array[0] = (uint8_t)((Color & 0x000F) << 4) | 0x0F;
             break;
        }
      #endif

      #ifdef GRAFX_COLOR_RGB565
        case PIXEL_FORMAT_RGB565:
        {
             _Color.u8_Array[3] = 0xFF;
             _Color.u8_Array[2] = (uint8_t)((Color & 0xF800) >> 8) | 0x07;
             _Color.u8_Array[1] = (uint8_t)((Color & 0x07E0) >> 3) | 0x03;
             _Color.u8_Array[0] = (uint8_t)((Color & 0x001F) << 3) | 0x07;
            break;
        }
      #endif

      #ifdef GRAFX_COLOR_ARGB1555
        case PIXEL_FORMAT_ARGB1555:
        {
             _Color.u8_Array[3] = (Color & 0x8000) ? 0xFF : 0x00;
             _Color.u8_Array[2] = (uint8_t)((Color & 0x7C00) >> 7) | 0x07;
             _Color.u8_Array[1] = (uint8_t)((Color & 0x03E0) >> 2) | 0x07;
             _Color.u8_Array[0] = (uint8_t)((Color & 0x001F) << 3) | 0x07;
            break;
        }
      #endif

      #ifdef GRAFX_COLOR_RGB888
        case PIXEL_FORMAT_RGB888:
        {
             _Color.u_32 = Color;
             _Color.u8_Array[3] = 0xFF;
            break;
        }
      #endif

        // TO DO
      #ifdef GRAFX_COLOR_L8
        case PIXEL_FORMAT_L8:        // Lookup table  16 color
      #endif
      #ifdef GRAFX_COLOR_AL44
        case PIXEL_FORMAT_AL44:      // Lookup table  16 alpha level,  16 color
      #endif
      #ifdef GRAFX_COLOR_AL88
        case PIXEL_FORMAT_AL88:      // Lookup table  256 alpha level, 256 color
      #endif
      #ifdef GRAFX_COLOR_ARGB8888
        case PIXEL_FORMAT_ARGB8888:                                  // Nothing to do
      #endif
      #ifdef GRAFX_COLOR_L4
        case PIXEL_FORMAT_L4:
      #endif
      #ifdef GRAFX_COLOR_A8
        case PIXEL_FORMAT_A8:
      #endif
      #ifdef GRAFX_COLOR_A4
        case PIXEL_FORMAT_A4:
      #endif
      #ifdef GRAFX_COLOR_RGB332
        case PIXEL_FORMAT_RGB332:
      #endif
      #ifdef GRAFX_COLOR_RGB444
        case PIXEL_FORMAT_RGB444:
      #endif
        default:
        {
            _Color.u_32 = Color;
            break;
        }
    }

    return _Color.u_32;
}

//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX


