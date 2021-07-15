//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_240x320 - SF-TC240-9370.cpp
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
#include "lib_grafx.h"
#include "stm32f4xx.h"
#include "lib_class_STM32F4_i2c.h"
#include "bsp.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// private const(s)
//-------------------------------------------------------------------------------------------------

static const uint32_t DRV_PixelFormatTable[GRAFX_PIXEL_FORMAT_COUNT] =
{
    LTDC_Pixelformat_ARGB8888,
    LTDC_Pixelformat_RGB888,
    LTDC_Pixelformat_RGB565,
    LTDC_Pixelformat_ARGB1555,
    LTDC_Pixelformat_ARGB4444,
    LTDC_Pixelformat_L8,
    LTDC_Pixelformat_AL44,
    LTDC_Pixelformat_AL88,
    LTDC_Pixelformat_L4,
    LTDC_Pixelformat_A8,
    LTDC_Pixelformat_A4,
};

const LTDC_Layer_TypeDef* DRV_LTDC_baLayer[GRAFX_NUMBER_OF_ACTIVE_LAYER] =
{
    LTDC_Layer1,
    LTDC_Layer2
};

//-------------------------------------------------------------------------------------------------
// private variable(s)
//-------------------------------------------------------------------------------------------------

static bool bDriverInitialize = false;

//-------------------------------------------------------------------------------------------------
// Function(s)
//-------------------------------------------------------------------------------------------------

//static void     DRV_Line                       (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction);
static void     DRV_GPIO_Config                 (void);

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_Copy
//
//  Parameter(s):   void*           pSrc
//                  Box_t*          pBox
//                  Cartesian_t*    pDstPos
//                  PixelFormat_e   SrcPixelFormat_e
//                  BlendMode_e     BlendMode
//  Return:         None
//
//  Description:    Copy a rectangle region from square memory region to another square memory
//                  region
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void DRV_Copy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat_e, BlendMode_e BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormatSrc     = DRV_PixelFormatTable[SrcPixelFormat_e];
    PixelFormatDst     = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pDstPos->Y * GRAFX_SIZE_X) + pDstPos->X) * (uint32_t)PixelSize);

    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;


    DMA2D->CR          = (BlendMode == CLEAR_BLEND) ? 0x00000000UL : 0x00020000UL;                                  // Memory to memory and TCIE blending BG + Source
    DMA2D->CR         |= (1 << 9);

    //Source
    DMA2D->FGMAR       = (uint32_t)(pSrc) +(((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);    // Source address
    DMA2D->FGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Source line offset none as we are linear
    DMA2D->FGPFCCR     = PixelFormatSrc;                                                                            // Defines the size of pixel

    // Source
    DMA2D->BGMAR       = Address;                                                                                   // Source address
    DMA2D->BGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Source line offset
    DMA2D->BGPFCCR     = PixelFormatDst;                                                                            // Defines the size of pixel

    //Destination
    DMA2D->OMAR        = Address;                                                                                   // Destination address
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Destination line offset none as we are linear
    DMA2D->OPFCCR      = PixelFormatDst;                                                                            // Defines the size of pixel

    DMA2D->NLR         = AreaConfig.u_32;                                                                           // Size configuration of area to be transfered
    DMA2D->CR         |= 1;                                                                                         // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                                                                              // Wait until transfer is done
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_CopyLinear
//
//  Parameter(s):   void*           pSrc
//                  Box_t*          pBox
//                  PixelFormat_e   SrcPixelFormat_e)
//                  BlendMode_e     BlendMode
//  Return:         None
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void DRV_CopyLinear(void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat_e, BlendMode_e BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormatSrc     = DRV_PixelFormatTable[SrcPixelFormat_e];
    PixelFormatDst     = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);

    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

    DMA2D->CR          = (BlendMode == CLEAR_BLEND) ? 0x00000000UL : 0x00020000UL;         // Memory to memory and TCIE blending BG + Source
    DMA2D->CR         |= (1 << 9);

    // Source
    DMA2D->FGMAR       = (uint32_t)pSrc;                                                   // Source address
    DMA2D->FGOR        = 0;                                                                // Source line offset none as we are linear
    DMA2D->FGPFCCR     = PixelFormatSrc;                                                   // Defines the size of pixel

    // Source
    DMA2D->BGMAR       = Address;                                                          // Source address
    DMA2D->BGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;              // Source line offset
    DMA2D->BGPFCCR     = PixelFormatDst;                                                   // Defines the size of pixel

    // Destination
    DMA2D->OMAR        = Address;                                                          // Destination address
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;              // Destination line offset
    DMA2D->OPFCCR      = PixelFormatDst;                                                   // Defines the size of pixel

    DMA2D->NLR         = AreaConfig.u_32;                                                  // Size configuration of area to be transfered
    DMA2D->CR         |= 1;                                                                // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                                                     // Wait until transfer is done
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawRectangle
//
//  Parameter(s):   Box_t*   pBox
//  Return:         None
//
//  Description:    Fill a region in a specific color
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DrawRectangle(Box_t* pBox)
{
    uint32_t           PixelFormat;
    uint32_t           Address;
    uint32_t           Color;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormat        = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);
    Color              = pLayer->GetColor();
    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

    DMA2D->CR          = 0x00030000UL | (1 << 9);                              // Register to memory and TCIE
    DMA2D->OCOLR       = Color;                                                // Color to be used
    DMA2D->OMAR        = Address;                                              // Destination address
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;  // Destination line offset
    DMA2D->OPFCCR      = PixelFormat;                                          // Defines the number of pixels to be transfered
    DMA2D->NLR         = AreaConfig.u_32;                                      // Size configuration of area to be transfered
    DMA2D->CR         |= 1;                                                    // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                                         // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawBox
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
void DRV_DrawBox(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness)
{
    uint16_t X2 = PosX + Length;
    uint16_t Y2 = PosY + Height;

    DRV_DrawVLine(PosX,           PosY, Y2, Thickness);
    DRV_DrawVLine(X2 - Thickness, PosY, Y2, Thickness);
    DRV_DrawHLine(PosY,           PosX, X2, Thickness);
    DRV_DrawHLine(Y2 - Thickness, PosX, X2, Thickness);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawPixel
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
void DRV_DrawPixel(uint16_t PosX, uint16_t PosY)
{
    uint32_t       PixelFormat;
    uint32_t       Address;
    uint32_t       Color;
    CLayer*        pLayer;
    uint8_t        PixelSize;

    pLayer         = &LayerTable[CLayer::GetDrawing()];
    PixelFormat    = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize      = pLayer->GetPixelSize();
    Address        = pLayer->GetAddress() + (((PosY * GRAFX_SIZE_X) + PosX) * (uint32_t)PixelSize);
    Color          = pLayer->GetColor();

    DMA2D->CR      = 0x00030000UL | (1 << 9);                   // Register to memory and TCIE
    DMA2D->OCOLR   = Color;                                     // Color to be used
    DMA2D->OMAR    = Address;                                   // Destination address
    DMA2D->OPFCCR  = PixelFormat;                               // Defines the number of pixels to be transfered
    DMA2D->NLR     = 0x00010001;                                // Size configuration of area to be transfered
    DMA2D->CR     |= 1;                                         // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                          // Wait until transfer is done
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawHLine
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
void DRV_DrawHLine(uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t Thickness)
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

    DRV_DrawLine(PosX1, PosY, Length, Thickness, DRAW_HORIZONTAL);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawVLine
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
void DRV_DrawVLine(uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t Thickness)
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

    DRV_DrawLine(PosX, PosY1, Length, Thickness, DRAW_VERTICAL);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawLine
//
//  Parameter(s):   uint16_t   PosX       Specifies the X position, can be a value from 0 to 240.
//                  uint16_t   PosY       Specifies the Y position, can be a value from 0 to 320.
//                  uint16_t   Length     Line length.
//                  uint16_t   ThickNess
//                  DrawMode_e Direction  line direction.
//                                        This Parameter can be one of the following values
//                                                 DRAW_HORIZONTAL
//                                                 DRAW_VERTICAL
//  Return:         None
//
//  Description:    Displays a line of a specific thickness.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DrawLine(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction)
{
    uint32_t      PixelFormat;
    uint8_t       PixelSize;
    uint32_t      Address;
    uint32_t      Color;
    CLayer*       pLayer;
    s32_t         AreaConfig;

    pLayer        = &LayerTable[CLayer::GetDrawing()];
    PixelFormat   = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize     = pLayer->GetPixelSize();
    Address       = pLayer->GetAddress() + (((PosY * GRAFX_SIZE_X) + PosX) * (uint32_t)PixelSize);
    Color         = pLayer->GetColor();

    if(Direction == DRAW_HORIZONTAL)
    {
        AreaConfig.u_16.u1 = Length;
        AreaConfig.u_16.u0 = ThickNess;
    }
    else
    {
        AreaConfig.u_16.u1 = ThickNess;
        AreaConfig.u_16.u0 = Length;
    }

    DMA2D->CR      = 0x00030000UL | (1 << 9);                               // Register to memory and TCIE
    DMA2D->OCOLR   = Color;                                                 // Color to be used
    DMA2D->OMAR    = Address;                                               // Destination address
    DMA2D->OOR     = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1; // Destination line offset
    DMA2D->OPFCCR  = PixelFormat;                                           // Defines the number of pixels to be transfered
    DMA2D->NLR     = AreaConfig.u_32;                                       // Size configuration of area to be transfered
    DMA2D->CR     |= 1;                                                     // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                                      // Wait until transfer is done
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_PrintFont
//
//  Parameter(s):   FONT_sDescriptor*   pDescriptor
//                  Cartesian_t*        pPos
//  Return:         none
//
//  Description:    This function will print a font to drawing layer with the drawing color
//
//-------------------------------------------------------------------------------------------------
void DRV_PrintFont(FontDescriptor_t* pDescriptor, Cartesian_t* pPos)
{
    uint32_t           PixelFormat;
    uint8_t            PixelSize;
    uint32_t           Address;
    CLayer*            pLayer;
    PixelFormat_e       PixFmt;
    s32_t              AreaConfig;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixFmt             = pLayer->GetPixelFormat();
    PixelFormat        = DRV_PixelFormatTable[PixFmt];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pPos->Y * GRAFX_SIZE_X) + pPos->X) * (uint32_t)PixelSize);
    AreaConfig.u_16.u1 = pDescriptor->Size.Width;
    AreaConfig.u_16.u0 = pDescriptor->Size.Height;

    DMA2D->CR          = 0x00020000UL | (1 << 9);                                   // Memory to memory and TCIE

    // Font layer in Alpha blending linear (A8)
    DMA2D->FGMAR       = (uint32_t)pDescriptor->pAddress;                           // Source address 1
    DMA2D->FGOR        = 0;                                                         // Font source line offset - none as we are linear
    DMA2D->FGCOLR      = pLayer->GetTextColor();
    DMA2D->FGPFCCR     = LTDC_Pixelformat_A8;                                       // Defines the number of pixels to be transfered

    DMA2D->BGMAR       = Address;                                                   // Source address 2
    DMA2D->BGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Font source line offset - none as we are linear
    DMA2D->BGPFCCR     = PixelFormat;                                               // Defines the number of pixels to be transfered

    // Output Layer
    DMA2D->OMAR        = Address;
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Destination line offset
    DMA2D->OPFCCR      = PixelFormat;

    // Area
    DMA2D->NLR         = AreaConfig.u_32;                                           // Size configuration of area to be transfered
    DMA2D->CR         |= 1;                                                         // Start operation

    while(DMA2D->CR & DMA2D_CR_START);                                              // Wait until transfer is done
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    LCD configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_Initialize()
{
    LTDC_InitTypeDef LTDC_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);                                           // Enable the DMA2D Clock for transfert

    // ---- Enable Pixel Clock ----
    // Configure PLLSAI prescalers for LCD
    // PLLSAI_VCO Input                                             = 1 Mhz
    // PLLSAI_VCO Output    = PLLSAI_VCO Input * PLLSAI_N  = 1*192  = 192 Mhz
    // PLLLCDCLK            = PLLSAI_VCO Output/PLLSAI_R   = 192/3  = 64 Mhz
    // LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR   = 64/8   = 8.0 Mhz
    RCC_PLLSAIConfig(192, 7, 3);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

    RCC_PLLSAICmd(ENABLE);                                                                          // Enable PLLSAI Clock
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET){};                                        // Wait for PLLSAI activation

    // ---- LTDC Initialization ----
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);                                            // Enable the LTDC Clock

    DRV_GPIO_Config();                                                                              // Configure the LCD Control pins

    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;                                           // Initialize the horizontal synchronization polarity as active low
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;                                           // Initialize the vertical synchronization polarity as active low
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;                                           // Initialize the data enable polarity as active low
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;                                          // Initialize the pixel clock polarity as input pixel clock

    // Timing configuration
    LTDC_InitStruct.LTDC_HorizontalSync     = GRAFX_HSYNC;                                          // Configure horizontal synchronization width
    LTDC_InitStruct.LTDC_VerticalSync       = GRAFX_VSYNC;                                          // Configure vertical synchronization height
    LTDC_InitStruct.LTDC_AccumulatedHBP     = GRAFX_HSYNC + GRAFX_HBP;                              // Configure accumulated horizontal back porch
    LTDC_InitStruct.LTDC_AccumulatedVBP     = GRAFX_VSYNC + GRAFX_VBP;                              // Configure accumulated vertical back porch
    LTDC_InitStruct.LTDC_AccumulatedActiveW = GRAFX_HSYNC + GRAFX_HBP + GRAFX_SIZE_X;               // Configure accumulated active width
    LTDC_InitStruct.LTDC_AccumulatedActiveH = GRAFX_VSYNC + GRAFX_VBP + GRAFX_SIZE_Y;               // Configure accumulated active height
    LTDC_InitStruct.LTDC_TotalWidth         = GRAFX_HSYNC + GRAFX_HBP + GRAFX_SIZE_X + GRAFX_HFP;   // Configure total width
    LTDC_InitStruct.LTDC_TotalHeigh         = GRAFX_VSYNC + GRAFX_VBP + GRAFX_SIZE_Y + GRAFX_VFP;   // Configure total height

    // Configure R,G,B component values for LCD background color
    LTDC_InitStruct.LTDC_BackgroundRedValue   = 0;
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
    LTDC_InitStruct.LTDC_BackgroundBlueValue  = 0;

    LTDC_Init(&LTDC_InitStruct);                                                                    // Initialize LTDC

    //LTDC_DitherCmd(ENABLE);

    bDriverInitialize = true;

    // Reload configuration
    LTDC_ReloadConfig(LTDC_IMReload);

    // Enable The LCD
    LTDC_Cmd(ENABLE);
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
    LTDC_Layer_InitTypeDef      LTDC_Layer_InitStruct;
    uint8_t                     PixelSize;
    LTDC_Layer_TypeDef*         pLTDC_SelectedLayer;
    LayerType_e                 ActiveLayer;

    ActiveLayer = pLayer->GetActive();

    if(ActiveLayer < GRAFX_NUMBER_OF_ACTIVE_LAYER)
    {
        pLTDC_SelectedLayer = (LTDC_Layer_TypeDef*)DRV_LTDC_baLayer[ActiveLayer];
        PixelSize           = pLayer->GetPixelSize();

        //      Windowing configuration
        //          All the active display area is used to display
        //          Horizontal start = horizontal synchronization + Horizontal back porch + 1
        //          Horizontal stop  = Horizontal start + window width
        //          Vertical start   = vertical synchronization + vertical back porch + 1
        //          Vertical stop    = Vertical start + window height
        LTDC_Layer_InitStruct.LTDC_HorizontalStart   = GRAFX_HSYNC + GRAFX_HBP + 1;
        LTDC_Layer_InitStruct.LTDC_HorizontalStop    = GRAFX_HSYNC + GRAFX_HBP + GRAFX_SIZE_X;
        LTDC_Layer_InitStruct.LTDC_VerticalStart     = GRAFX_VSYNC + GRAFX_VBP + 1;
        LTDC_Layer_InitStruct.LTDC_VerticalStop      = GRAFX_VSYNC + GRAFX_VBP + GRAFX_SIZE_Y;
        LTDC_Layer_InitStruct.LTDC_PixelFormat       = DRV_PixelFormatTable[pLayer->GetPixelFormat()];     // Pixel Format configuration
        LTDC_Layer_InitStruct.LTDC_ConstantAlpha     = (uint32_t)pLayer->GetAlpha();                        // Alpha constant (255 totally opaque)
        LTDC_Layer_InitStruct.LTDC_BlendingFactor_1  = LTDC_BlendingFactor1_PAxCA;                          // Configure blending factors
        LTDC_Layer_InitStruct.LTDC_BlendingFactor_2  = LTDC_BlendingFactor2_PAxCA;
        LTDC_Layer_InitStruct.LTDC_DefaultColorBlue  = 0;                                                   // Default Color configuration (configure A,R,G,B component values)
        LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
        LTDC_Layer_InitStruct.LTDC_DefaultColorRed   = 0;
        LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
        LTDC_Layer_InitStruct.LTDC_CFBStartAdress    = pLayer->GetAddress();                                // Input Address configuration

        // the length of one line of pixels in bytes + 3 then :
        // Line Lenth = Active high width x number of bytes per pixel + 3
        // Active high width         = LCD_SIZE_Y
        LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((GRAFX_SIZE_X * (uint32_t)PixelSize) + 3);

        // the pitch is the increment from the start of one line of pixels to the
        // start of the next line in bytes, then :
        // Pitch = Active high width x number of bytes per pixel
        LTDC_Layer_InitStruct.LTDC_CFBPitch      = (GRAFX_SIZE_X * (uint32_t)PixelSize);
        LTDC_Layer_InitStruct.LTDC_CFBLineNumber =  GRAFX_SIZE_Y;                                              // Configure the number of lines

        LTDC_LayerInit(pLTDC_SelectedLayer, &LTDC_Layer_InitStruct);

        if(bDriverInitialize == true)
        {
            LTDC_LayerCmd(pLTDC_SelectedLayer, ENABLE);
        }

        LTDC_ReloadConfig(LTDC_VBReload);
    }
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
#ifdef GRAFX_USE_V_SYNC
void DRV_WaitFor_V_Sync(void)
{
    // Wait for Vertical sync to occur
    while(LTDC_GetCDStatus(LTDC_CDSR_VSYNCS) != SET);
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_GPIO_Config
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    GPIO configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Enable GPIOA, GPIOI, GPIOJ, GPIOJ AHB Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD |
                           RCC_AHB1Periph_GPIOF |
                           RCC_AHB1Periph_GPIOG, ENABLE);

    // +------------------------+-----------------------+----------------------------+
    // +                       LCD pins assignment                                   +
    // +------------------------+-----------------------+----------------------------+
    // |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
    // |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
    // |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
    // |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
    // |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
    // |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
    // -------------------------------------------------------------------------------
    // |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
    // |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
    // |  LCD_TFT C/D   <-> PD.13  |                         |
    // -------------------------------------------------------

    // ---- GPIOs Configuration ----

    // GPIOs All other pin are configure the same
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    // GPIOA configuration
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // GPIOB configuration
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,  GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // GPIOC configuration
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,  GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // GPIOD configuration
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStruct);



    // GPIOF configuration
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

    // GPIOG configuration
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12,  GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStruct);

    // The following pin are output
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

    // Port C pin 2 is a simple output to control CS on SPI interface
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Port D pin 13 is a simple output to control C/D pin on LCD
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

