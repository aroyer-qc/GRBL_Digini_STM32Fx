//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_128x128-SSD1779.h
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
#include "lib_grafx.h"
#include "lib_lcd_driver.h"
#include "lib_io.h"
#include "lib_class_io_bus.h"
#include "stm32f4xx.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define GRAFX_NUMBER_OF_ACTIVE_LAYER              2

//#define GRAFX_USE_V_SYNC
//#define GRAFX_USE_SOFT_PIXEL
//#define GRAFX_USE_SOFT_BOX
//#define GRAFX_USE_SOFT_VLINE
//#define GRAFX_USE_SOFT_HLINE
//#define GRAFX_USE_SOFT_DLINE
#define GRAFX_USE_SOFT_CIRCLE

//#define GRAFX_USE_SOFT_PRINT_FONT
#define GRAFX_USE_SOFT_ALPHA
//#define GRAFX_USE_SOFT_COPY
//#define GRAFX_USE_SOFT_FILL

// Display size
#define GRAFX_SIZE_X            128
#define GRAFX_SIZE_Y            128

//#define GRAFX_DRIVER_USE_OWN_COPY_LAYER_TO_LAYER

#define SSD1779_NO_FILL         0
#define SSD1779_FILL            1

//-------------------------------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------------------------------

class GrafxDriver : public GRAFX_Interface
{
    public:

        void            Initialize            (void* pArg);
        void            DisplayOn             (void);
        void            DisplayOff            (void);
        void            LayerConfig           (CLayer* pLayer);
        void            CopyLinear            (void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
        void            BlockCopy             (void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
        void            BlockCopy             (void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t DstX, uint16_t DstY, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);
        void            DrawRectangle         (Box_t* pBox);
        void            PrintFont             (FontDescriptor_t* pDescriptor, Cartesian_t* pPos);
        void            DrawBox               (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness);
        void            DrawPixel             (uint16_t PosX, uint16_t PosY);
        void            DrawHLine             (uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t ThickNess);
        void            DrawVLine             (uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t ThickNess);
        void            DrawLine              (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Thickness, DrawMode_e Direction);
        void            DrawCircle            (uint8_t X, uint8_t Y, uint8_t Radius, uint8_t Mode);
        void            Copy                  (void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat_e, BlendMode_e BlendMode);

        #ifdef GRAFX_USE_V_SYNC
        void            WaitFor_V_Sync        (void) {};
        #endif

        void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, Box_t* pBox) {};
        void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height) {};
        void            CopyLayerToLayer      (Layer_e SrcLayer, Layer_e DstLayer, uint16_t SrcX, uint16_t SrcY, uint16_t DstX, uint16_t DstY, uint16_t Width, uint16_t Height) {};

    private:

        void            ControllerInitialize  (void);     // LCD is equipped with a IL9341 controller
        void            WriteCommand          (uint8_t Register);
        uint8_t         ReadCommand           (uint8_t Register);
        void            WriteData             (uint8_t Data);
        void            WriteData             (uint16_t Data);
        uint8_t         ReadData_8            (void);
        uint16_t        ReadData_16           (void);
        void            Line                  (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction);
        void            DrawRectangle         (Box_t* pBox, uint8_t Mode = SSD1779_NO_FILL);
        void            Clear                 (void);

        IO_ID_e         m_RegSelect;
        IO_ID_e         m_Reset;
        IO_BusDriver    m_Bus;
        CLayer*         m_pLayer;
};

//-------------------------------------------------------------------------------------------------

#ifdef LIB_SSD1779_GLOBAL
class GrafxDriver             Grafx;
class GRAFX_Interface*        myGrafx = &Grafx;
#else
extern class GRAFX_Interface* myGrafx;
#endif

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

