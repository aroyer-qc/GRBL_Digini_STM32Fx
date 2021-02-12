//-------------------------------------------------------------------------------------------------
//
//  File : lib_lcd_driver.h
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
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#ifndef GRAFX_USE_V_SYNC
#define DRV_WaitFor_V_Sync(void)
#endif

//-------------------------------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------------------------------

class GRAFX_Interface
{
    public:

        virtual void            Initialize            (void* pArg)                                                                                                                                             = 0;
        virtual void            DisplayOn             (void)                                                                                                                                                   = 0;
        virtual void            DisplayOff            (void)                                                                                                                                                   = 0;
        virtual void            LayerConfig           (CLayer* pLayer)                                                                                                                                         = 0;
        virtual void            CopyLinear            (void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)                                                                           = 0;
        virtual void            BlockCopy             (void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)                                                     = 0;
        virtual void            BlockCopy             (void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t DstX, uint16_t DstY, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode) = 0;
        virtual void            DrawRectangle         (Box_t* pBox)                                                                                                                                            = 0;
        virtual void            PrintFont             (FontDescriptor_t* pDescriptor, Cartesian_t* pPos)                                                                                                       = 0;
        virtual void            DrawBox               (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness)                                                                     = 0;
        virtual void            DrawPixel             (uint16_t PosX, uint16_t PosY)                                                                                                                           = 0;
        virtual void            DrawHLine             (uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t ThickNess)                                                                                      = 0;
        virtual void            DrawVLine             (uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t ThickNess)                                                                                      = 0;
        virtual void            DrawLine              (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Thickness, DrawMode_e Direction)                                                                = 0;
        virtual void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, Box_t* pBox)                                                                                                        = 0;
        virtual void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)                                                            = 0;
        virtual void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, uint16_t SrcX, uint16_t SrcY, uint16_t DstX, uint16_t DstY, uint16_t Width, uint16_t Height)                        = 0;
        #ifdef GRAFX_USE_V_SYNC
        virtual void            WaitFor_V_Sync        (void)                                                                                                                                                   = 0;
        #endif
};

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
