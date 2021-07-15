//-------------------------------------------------------------------------------------------------
//
//  File : lib_grafx_enum.h
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
// Typedef(s) and structure(s)
//-------------------------------------------------------------------------------------------------

enum PixelFormat_e
{
  #ifdef GRAFX_COLOR_ARGB8888
    PIXEL_FORMAT_ARGB8888,
  #endif
  #ifdef GRAFX_COLOR_RGB888
    PIXEL_FORMAT_RGB888,
  #endif
  #ifdef GRAFX_COLOR_RGB565
    PIXEL_FORMAT_RGB565,
  #endif
  #ifdef GRAFX_COLOR_ARGB1555
    PIXEL_FORMAT_ARGB1555,
  #endif
  #ifdef GRAFX_COLOR_ARGB4444
    PIXEL_FORMAT_ARGB4444,
  #endif
  #ifdef GRAFX_COLOR_L8
    PIXEL_FORMAT_L8,
  #endif
  #ifdef GRAFX_COLOR_AL44
    PIXEL_FORMAT_AL44,
  #endif
  #ifdef GRAFX_COLOR_AL88
    PIXEL_FORMAT_AL88,
  #endif
  #ifdef GRAFX_COLOR_L4
    PIXEL_FORMAT_L4,
  #endif
  #ifdef GRAFX_COLOR_A8
    PIXEL_FORMAT_A8,
  #endif
  #ifdef GRAFX_COLOR_A4
    PIXEL_FORMAT_A4,
  #endif
  #ifdef GRAFX_COLOR_RGB332
    PIXEL_FORMAT_RGB332,
  #endif
  #ifdef GRAFX_COLOR_RGB444
    PIXEL_FORMAT_RGB444,
  #endif

    DUMMY_PIXEL_FORMAT,
    PIXEL_FORMAT_COUNT,
};

enum PolygonMode_e
{
    POLY_FILL = 0,
    POLY_SHAPE,
};

enum DrawMode_e
{
    DRAW_COPY_LINEAR_TO_BLOCK = 0,
    DRAW_COPY_BLOCK_TO_BLOCK,
    DRAW_HORIZONTAL,
    DRAW_VERTICAL,
};

enum BlendMode_e
{
    CLEAR_BLEND = 0,
    ALPHA_BLEND,
    NO_BLEND,
};

enum SkinBlockType_e
{
    SKIN_IMAGE,
    SKIN_FONT,
};

// If new service added here don't forget to adjust const -> ServiceSizeType
enum ServiceType_e
{
    SERVICE_RETURN            = 0,
    SERVICE_RETURN_TYPE1,
    SERVICE_RETURN_TYPE2,
    SERVICE_RETURN_TYPE3,
    SERVICE_RETURN_TYPE4,
    SERVICE_RETURN_TYPE5,
    SERVICE_RETURN_TYPE6,
    SERVICE_RETURN_TYPE7,
    SERVICE_RETURN_TYPE8,
    SERVICE_RETURN_INVALID_LINK,
    NB_SERVICE_CONST,
};


enum ServiceEvent_e
{
    SERVICE_NO_STATE                    =  0x00000000,
    SERVICE_NO_GESTURE                  =  0x00000000,
    SERVICE_NO_EVENT                    =  0x00000000,
    SERVICE_RELEASED                    =  0x00000001,
    SERVICE_IDLE                        =  0x00000002,
    SERVICE_PRESSED                     =  0x00000004,
    SERVICE_TRANSIT                     =  0x00000008,
    SERVICE_TYPEMATIC                   =  0x00000014,
    SERVICE_SUPERKEY                    =  0x00000024,
    SERVICE_CANCEL                      =  0x00000040,
    SERVICE_MOVE_UP                     =  0x00000080,
    SERVICE_MOVE_DOWN                   =  0x00000100,
    SERVICE_MOVE_LEFT                   =  0x00000200,
    SERVICE_MOVE_RIGHT                  =  0x00000400,
    SERVICE_SINGLE_CLICK                =  0x00000800,
    SERVICE_DOUBLE_CLICK                =  0x00001000,
    SERVICE_ROTATE_CLOCKWISE            =  0x00002000,
    SERVICE_ROTATE_COUNTER_CLOCKWISE    =  0x00004000,
    SERVICE_ZOOM_IN                     =  0x00008000,
    SERVICE_ZOOM_OUT                    =  0x00010000,
    //SERVICE_CONTACT                     =  0x00020000,
    //SERVICE_SAME_EVENT                  =  0x00040000, // found out if there is a reason to have same value as service start...
    SERVICE_EXIT_EVENT                  =  0x00080000,
    SERVICE_START                       =  0x00020000,
    SERVICE_REFRESH                     =  0x00040000,
    SERVICE_FORCED                      =  0x00080000,            // this will be called when a dromant widget has to be refreshed (ex: windows over widget)
    SERVICE_FINALIZE                    =  0x00100000,
    SERVICE_SAME_EVENT                  =  0x00200000,  // tempo put a new vaue here
    SERVICE_INVALID                     =  0x80000000,
};

/*
SERVICE_DOUBLE_CLICK                =  0x00001000,
SERVICE_ROTATE_CLOCKWISE            =  0x00002000,
SERVICE_ROTATE_COUNTER_CLOCKWISE    =  0x00004000,
SERVICE_ZOOM_IN                     =  0x00008000,
SERVICE_ZOOM_OUT                    =  0x00010000,
SERVICE_CONTACT                     =  0x00020000,
SERVICE_SAME_EVENT                  =  0x00040000,
SERVICE_EXIT_EVENT                  =  0x00080000,
SERVICE_START                       =  0x00100000,
SERVICE_REFRESH                     =  0x00200000,
SERVICE_FORCED                      =  0x00400000,            // this will be called when a dormant widget has to be refreshed (ex: windows over widget)
*/

enum MessageType_e
{
    MESSAGE_TYPE_PDI_EVENT_INFO       = 0,
    MESSAGE_TYPE_PAGE_LOADED,
    MESSAGE_TYPE_LOAD_PAGE,
    MESSAGE_TYPE_REFRESH_PAGE,
};

enum LayerType_e
{
  #ifdef GRAFX_USE_BACKGROUND_LAYER
    LAYER_BACKGROUND,
  #endif
    LAYER_FOREGROUND,
    LAYER_VIRTUAL,
};

enum ExchangeType_e
{
    EXCHANGE_TYPE_NONE,
    EXCHANGE_INPUT_TYPE_DECIMAL,
    EXCHANGE_INPUT_TYPE_FLOAT,
    EXCHANGE_INPUT_TYPE_HEXA,
};

enum SlideDir_e
{
    SLIDING_NONE                = 0x00,
    SLIDING_LEFT                = 0x01,
    SLIDING_RIGHT               = 0x02,
    SLIDING_UP                  = 0x03,
    SLIDING_DOWN                = 0x04,
    SLIDING_MASK                = 0x07,

    SLIDING_LEFT_OVLAP          = 0x09,
    SLIDING_RIGHT_OVLAP         = 0x0A,
    SLIDING_UP_OVLAP            = 0x0B,
    SLIDING_DOWN_OVLAP          = 0x0C,
    SLIDING_OVERLAP_MASK        = 0x08,

    SLIDING_LEFT_DE_OVLAP       = 0x11,
    SLIDING_RIGHT_DE_OVLAP      = 0x12,
    SLIDING_UP_DE_OVLAP         = 0x13,
    SLIDING_DOWN_DE_OVLAP       = 0x14,
    SLIDING_DE_OVERLAP_MASK     = 0x10,
};


//-------------------------------------------------------------------------------------------------


