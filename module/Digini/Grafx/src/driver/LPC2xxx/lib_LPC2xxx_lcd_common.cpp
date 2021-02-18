//-------------------------------------------------------------------------------------------------
//
//  File : lib_LPC2xxx_lcd_common.cpp
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

#ifdef DIGINI_USE_GRAFX
#ifdef LPC2xx

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_lcd_driver.h"
#include "lib_grafx.h"
//#include "stm32f4xx_include.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_Copy
//
//  Parameter(s):   void*           pSrc
//                  sBox*           pBox
//                  sCartesian*     pDstPos
//                  ePixelFormat    SourcePixelFormat
//                  eBlendMode      BlendMode
//  Return:         None
//
//  Description:    Copy a rectangle region from square memory region to another square memory
//                  region
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_Copy(void* pSrc, sBox* pBox, sCartesian* pDstPos, ePixelFormat SourcePixelFormat, eBlendMode BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[GetDrawingLayer()];
    PixelFormatSrc     = PixelFormatTable[SourcePixelFormat];
    PixelFormatDst     = PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pDstPos->Y * GRAFX_SIZE_X) + pDstPos->X) * (uint32_t)PixelSize);

    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

// specific processor code
    
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_CopyLinear
//
//  Parameter(s):   void*           pSrc
//                  sBox*           pBox
//                  ePixelFormat    SourcePixelFormat)
//                  eBlendMode      BlendMode
//  Return:         None
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_CopyLinear(void* pSrc, sBox* pBox, ePixelFormat SourcePixelFormat, eBlendMode BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[GetDrawingLayer()];
    PixelFormatSrc     = PixelFormatTable[SourcePixelFormat];
    PixelFormatDst     = PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);

    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

// specific processor code
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawRectangle
//
//  Parameter(s):   sBox*   pBox
//  Return:         None
//
//  Description:    Fill a region in a specific color
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_DrawRectangle(sBox* pBox)
{
    uint32_t           PixelFormat;
    uint32_t           Address;
    uint32_t           Color;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[GetDrawingLayer()];
    PixelFormat        = PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);
    Color              = pLayer->GetColor();
    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

    // specific processor code
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawBox
//
//  Parameter(s):   uint16_t    PosX
//                  uint16_t    PosY
//                  uint16_t    Length
//                  uint16_t    Height
//                  uint16_t    Thickness
//  Return:         None
//
//  Description:    Draw a box in a specific thickness
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------

// TODO move elsewhere this is generic code

void GRAFX_DRV_DrawBox(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness)
{
    uint16_t X2 = PosX + Length;
    uint16_t Y2 = PosY + Height;

    GRAFX_DRV_DrawVLine(PosX,           PosY, Y2, Thickness);
    GRAFX_DRV_DrawVLine(X2 - Thickness, PosY, Y2, Thickness);
    GRAFX_DRV_DrawHLine(PosY,           PosX, X2, Thickness);
    GRAFX_DRV_DrawHLine(Y2 - Thickness, PosX, X2, Thickness);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawPixel
//
//  Parameter(s):   uint16_t    PosX
//                  uint16_t    PosY
//  Return:         None
//
//  Description:    Put a pixel on selected layer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_DrawPixel(uint16_t PosX, uint16_t PosY)
{
    uint32_t       PixelFormat;
    uint32_t       Address;
    uint32_t       Color;
    CLayer*        pLayer;
    uint8_t        PixelSize;

    pLayer         = &LayerTable[GetDrawingLayer()];
    PixelFormat    = PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize      = pLayer->GetPixelSize();
    Address        = pLayer->GetAddress() + (((PosY * GRAFX_SIZE_X) + PosX) * (uint32_t)PixelSize);
    Color          = pLayer->GetColor();

    // specific processor code
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawHLine
//
//  Parameter(s):   uint16_t    Y
//                  uint16_t    X1
//                  uint16_t    X2
//                  uint16_t    ThickNess
//  Return:         None
//
//  Description:    Displays a horizontal line of a specific thickness.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------

// TODO move elsewhere this is generic code

void GRAFX_DRV_DrawHLine(uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t Thickness)
{
    uint16_t Length;

    // X1 need to be the lowest (STM32 need this)
    if(PosX1 > PosX2)
    {
        Length = PosX1 - PosX2;
        PosX1  = PosX2;
    }
    else
    {
        Length = PosX2 - PosX1;
    }

    GRAFX_DRV_DrawLine(PosX1, PosY, Length, Thickness, DRAW_HORIZONTAL);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawVLine
//
//  Parameter(s):   uint16_t    wPosX
//                  uint16_t    wPosY1
//                  uint16_t    wPosY2
//                  uint16_t    wThickNess
//  Return:         None
//
//  Description:    Displays a vertical line of a specific thickness.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------

// TODO move elsewhere this is generic code

void GRAFX_DRV_DrawVLine(uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t Thickness)
{
    uint16_t Length;

    // Y1 need to be the lowest (STM32 need this)
    if(PosY1 > PosY2)
    {
        Length = PosY1 - PosY2;
        PosY1  = PosY2;
    }
    else
    {
        Length = PosY2 - PosY1;
    }

    GRAFX_DRV_DrawLine(PosX, PosY1, Length, Thickness, DRAW_VERTICAL);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_DrawLine
//
//  Parameter(s):   uint16_t  PosX       Specifies the X position, can be a value from 0 to 240.
//                  uint16_t  PosY       Specifies the Y position, can be a value from 0 to 320.
//                  uint16_t  Length     Line length.
//                  uint16_t  ThickNess
//                  eDrawMode Direction  line direction.
//                                       This Parameter can be one of the following values
//                                                 DRAW_HORIZONTAL
//                                                 DRAW_VERTICAL
//  Return:         None
//
//  Description:    Displays a line of a specific thickness.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_DrawLine(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Thickness, eDrawMode Direction)
{
    uint32_t      PixelFormat;
    uint8_t       PixelSize;
    uint32_t      Address;
    uint32_t      Color;
    CLayer*       pLayer;
    s32_t         AreaConfig;

    pLayer        = &LayerTable[GetDrawingLayer()];
    PixelFormat   = PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize     = pLayer->GetPixelSize();
    Address       = pLayer->GetAddress() + (((PosY * GRAFX_SIZE_X) + PosX) * (uint32_t)PixelSize);
    Color         = pLayer->GetColor();

    if(Direction == DRAW_HORIZONTAL)
    {
        AreaConfig.u_16.u1 = Length;
        AreaConfig.u_16.u0 = Thickness;
    }
    else
    {
        AreaConfig.u_16.u1 = Thickness;
        AreaConfig.u_16.u0 = Length;
    }

    // specific processor code
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_PrintFont
//
//  Parameter(s):   FONT_sDescriptor*   pDescriptor
//                  sCartesian*         pPos
//  Return:         none
//
//  Description:    This function will print a font to drawing layer with the drawing color
//
//-------------------------------------------------------------------------------------------------
void DRV_PrintFont(sFontDescriptor* pDescriptor, sCartesian* pPos)
{
    uint32_t           PixelFormat;
    uint8_t            PixelSize;
    uint32_t           Address;
    CLayer*            pLayer;
    ePixelFormat       PixFmt;
    s32_t              AreaConfig;

    pLayer             = &LayerTable[GetDrawingLayer()];
    PixFmt             = pLayer->GetPixelFormat();
    PixelFormat        = PixelFormatTable[PixFmt];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pPos->Y * GRAFX_SIZE_X) + pPos->X) * (uint32_t)PixelSize);
    AreaConfig.u_16.u1 = pDescriptor->Size.Width;
    AreaConfig.u_16.u0 = pDescriptor->Size.Height;

    // specific processor code
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_Init
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    LCD configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_Init()
{

    DRV_Config();                                                                             // Configure the LCD Control pins

//more than IO.. specific init for LCD

}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_LayerConfig
//
//  Parameter(s):   CLayer* pLayer
//  Return:         None
//
//  Description:    Configuration for layer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_LayerConfig(CLayer* pLayer)
{
// don't know if it apply to LPC
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DisplayOn
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Enables the Display
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DisplayOn()
{
    //GPIO_ResetBits(LCD_BL_CTRL_PORT, LCD_BL_CTRL_PIN);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DisplayOff
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Disables the Display
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DisplayOff()
{
    //GRAFX_GPIO_SetBits(LCD_BL_CTRL_PORT, LCD_BL_CTRL_PIN);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_WaitFor_V_Sync
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Wait for the vertical synchro
//
//  Note(s):        this will prevent glitching on screen while changing display
//
//-------------------------------------------------------------------------------------------------
void DRV_WaitFor_V_Sync()
{
    // Wait for Vertical sync to occur
}


#endif // LPC2XXX
#endif // DIGINI_USE_GRAFX
