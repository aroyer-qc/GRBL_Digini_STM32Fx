//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_lcd_480x272 - RK043FN48H.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#define LIB_RK043FN48H_GLOBAL
#include "lib_STM32F7_lcd_480x272 - RK043FN48H.h"
#undef  LIB_RK043FN48H_GLOBAL
#include "lib_io.h"
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define LTDC_PIXEL_FORMAT_ARGB8888      0
#define LTDC_PIXEL_FORMAT_RGB888        1
#define LTDC_PIXEL_FORMAT_RGB565        2
#define LTDC_PIXEL_FORMAT_ARGB1555      3
#define LTDC_PIXEL_FORMAT_ARGB4444      4
#define LTDC_PIXEL_FORMAT_L8            5
#define LTDC_PIXEL_FORMAT_AL44          6
#define LTDC_PIXEL_FORMAT_AL88          7
#define LTDC_PIXEL_FORMAT_L4            8
#define LTDC_PIXEL_FORMAT_A8            9
#define LTDC_PIXEL_FORMAT_A4            10

#define GRAFX_HSYNC                     41              // Horizontal synchronization
#define GRAFX_HBP                       13              // Horizontal back porch
#define GRAFX_HFP                       32              // Horizontal front porch
#define GRAFX_VSYNC                     10              // Vertical synchronization
#define GRAFX_VBP                       2               // Vertical back porch
#define GRAFX_VFP                       2               // Vertical front porch

#define GRAFX_PLLSAIN_VALUE             192
#define GRAFX_PLLSAIR_VALUE             5
#define GRAFX_PLLSAIQ_VALUE             7

#define LTDC_BLENDING_FACTOR1_PAxCA     0x00000600      // Blending factor: Cte Alpha x Pixel Alpha
#define LTDC_BLENDING_FACTOR2_PAxCA     0x00000007      // Blending factor: Cte Alpha x Pixel Alpha

#define DMA2D_M2M                       0               // DMA2D memory to memory transfer mode
#define DMA2D_M2M_PFC                   DMA2D_CR_MODE_0 // DMA2D memory to memory with pixel format conversion transfer mode
#define DMA2D_M2M_BLEND                 DMA2D_CR_MODE_1 // DMA2D memory to memory with blending transfer mode
#define DMA2D_R2M                       DMA2D_CR_MODE   // DMA2D register to memory transfer mode

//-------------------------------------------------------------------------------------------------
// const(s)
//-------------------------------------------------------------------------------------------------

static const int32_t DRV_PixelFormatTable[PIXEL_FORMAT_COUNT] =
{
  #ifdef GRAFX_COLOR_ARGB8888
    LTDC_PIXEL_FORMAT_ARGB8888,
  #endif
  #ifdef GRAFX_COLOR_RGB888
    LTDC_PIXEL_FORMAT_RGB888,
  #endif
  #ifdef GRAFX_COLOR_RGB565
    LTDC_PIXEL_FORMAT_RGB565,
  #endif
  #ifdef GRAFX_COLOR_ARGB1555
    LTDC_PIXEL_FORMAT_ARGB1555,
  #endif
  #ifdef GRAFX_COLOR_ARGB4444
    LTDC_PIXEL_FORMAT_ARGB4444,
  #endif
  #ifdef GRAFX_COLOR_L8
    LTDC_PIXEL_FORMAT_L8,
  #endif
  #ifdef GRAFX_COLOR_AL44
    LTDC_PIXEL_FORMAT_AL44,
  #endif
  #ifdef GRAFX_COLOR_AL88
    LTDC_PIXEL_FORMAT_AL88,
  #endif
  #ifdef GRAFX_COLOR_L4
    LTDC_Pixelformat_L4,
  #endif
  #ifdef GRAFX_COLOR_A8
    LTDC_Pixelformat_A8,
  #endif
  #ifdef GRAFX_COLOR_A4
    LTDC_Pixelformat_A4,
  #endif
  #ifdef GRAFX_COLOR_RGB332
    -1,
  #endif
  #ifdef GRAFX_COLOR_RGB444
    -1,
  #endif
    -1
};

//-------------------------------------------------------------------------------------------------
// Local Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           IO_Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    GPIO configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::IO_Initialize(void)
{
    // +------------------------+-----------------------+----------------------------+
    // +                       LCD pins assignment                                   +
    // +------------------------+-----------------------+----------------------------+
    // |  LCD_TFT R0 <-> PI.15  |  LCD_TFT G0 <-> PJ.07 |  LCD_TFT B0 <-> PE.04      |
    // |  LCD_TFT R1 <-> PJ.00  |  LCD_TFT G1 <-> PJ.08 |  LCD_TFT B1 <-> PJ.13      |
    // |  LCD_TFT R2 <-> PJ.01  |  LCD_TFT G2 <-> PJ.09 |  LCD_TFT B2 <-> PJ.14      |
    // |  LCD_TFT R3 <-> PJ.02  |  LCD_TFT G3 <-> PJ.10 |  LCD_TFT B3 <-> PJ.15      |
    // |  LCD_TFT R4 <-> PJ.03  |  LCD_TFT G4 <-> PJ.11 |  LCD_TFT B4 <-> PG.12      |
    // |  LCD_TFT R5 <-> PJ.04  |  LCD_TFT G5 <-> PK.00 |  LCD_TFT B5 <-> PK.04      |
    // |  LCD_TFT R6 <-> PJ.05  |  LCD_TFT G6 <-> PK.01 |  LCD_TFT B6 <-> PK.05      |
    // |  LCD_TFT R7 <-> PJ.06  |  LCD_TFT G7 <-> PK.02 |  LCD_TFT B7 <-> PK.06      |
    // -------------------------------------------------------------------------------
    // |  LCD_TFT HSYNC <-> PI.10  | LCDTFT VSYNC <->  PI.09 |
    // |  LCD_TFT CLK   <-> PI.14  | LCD_TFT DE   <->  PK.07 |
    // -------------------------------------------------------
    // |  LCD_DISP      <-> PI.12  |
    // |  LCD_BL_CTRL   <-> PK.03  |
    // -----------------------------
    // By port
    //
    // E        4           Color
    //
    // G       12           Color
    //
    // I     9-10           Sync
    //      12-14           Control
    //         15           Color
    //
    // J     0-11           Color
    //      13-15           Color
    //
    // K      0-2           Color
    // K        3           Control
    // K      4-7           Color


    /// ---- GPIOs Configuration ----
    for(uint32_t IO_Id = uint32_t(IO_LCD_TFT_R0); IO_Id <= uint32_t(IO_LCD_TFT_BL_CTRL); IO_Id++)
    {
        IO_PinInit(IO_ID_e(IO_Id));
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LCD_Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Initialize clock
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::LCD_Initialize(void)
{
    // RK043FN48H LCD clock configuration
    // PLLSAI_VCO Input = HSI_VALUE/PLL_M = 1 Mhz
    // PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz
    // PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz
    // LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz
    RCC->PLLSAICFGR  = (GRAFX_PLLSAIR_VALUE << RCC_PLLSAICFGR_PLLSAIR_Pos) |
                       (GRAFX_PLLSAIQ_VALUE << RCC_PLLSAICFGR_PLLSAIQ_Pos) |
                       (GRAFX_PLLSAIN_VALUE << RCC_PLLSAICFGR_PLLSAIN_Pos);

// TODO found a better way
     SET_BIT(RCC->PLLSAICFGR, 0x01 << RCC_PLLSAICFGR_PLLSAIP_Pos); // Enable the USB Clock and SDMMC
    SET_BIT(RCC->DCKCFGR2, 1 << RCC_DCKCFGR2_CK48MSEL_Pos);

    MODIFY_REG(RCC->DCKCFGR1, RCC_DCKCFGR1_PLLSAIDIVR, RCC_DCKCFGR1_PLLSAIDIVR_0); // PLLSA IDIVR 4;
    RCC->CR         |= RCC_CR_PLLSAION;
    while((RCC->CR & RCC_CR_PLLSAIRDY) == 0);

    RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;

    // Configures the HS, VS, DE and PC polarity
    LTDC->GCR = 0;

    // Sets Synchronization size
    MODIFY_REG(LTDC->SSCR,
               (LTDC_SSCR_VSH | LTDC_SSCR_HSW),
               (GRAFX_VSYNC - 1) | ((GRAFX_HSYNC - 1) << LTDC_SSCR_HSW_Pos));

    // Sets Accumulated Back porch
    MODIFY_REG(LTDC->BPCR,
               (LTDC_BPCR_AVBP | LTDC_BPCR_AHBP),
               (GRAFX_VSYNC  + GRAFX_VBP - 1) | ((GRAFX_HSYNC  + GRAFX_HBP - 1) << LTDC_BPCR_AHBP_Pos));

    // Sets Accumulated Active Width
    MODIFY_REG(LTDC->AWCR,
               (LTDC_AWCR_AAH | LTDC_AWCR_AAW),
               ((GRAFX_SIZE_Y + GRAFX_VSYNC + GRAFX_VBP - 1) |
                ((GRAFX_SIZE_X + GRAFX_HSYNC + GRAFX_HBP - 1) << LTDC_AWCR_AAW_Pos)));

    // Sets Total Width for vertical and horizontal
    MODIFY_REG(LTDC->TWCR,
               (LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW),
               ((GRAFX_SIZE_Y + GRAFX_VSYNC + GRAFX_VBP + GRAFX_VFP - 1) |
                ((GRAFX_SIZE_X + GRAFX_HSYNC + GRAFX_HBP + GRAFX_HFP - 1) << LTDC_TWCR_TOTALW_Pos)));

    CLEAR_BIT(LTDC->BCCR, (LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED));    // Sets the background color value to zero for all
    SET_BIT(LTDC->IER, LTDC_IER_TERRIE | LTDC_IER_FUIE);                                // Enable the transfer Error interrupt and FIFO underrun
    SET_BIT(LTDC->GCR, LTDC_GCR_LTDCEN);                                                // Enable LTDC by setting LTDCEN bit
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LayerConfig
//
//  Parameter(s):   CLayer* pLayer
//  Return:         None
//
//  Description:    Configuration for layer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::LayerInitialize(void)
{
    LayerConfig(&LayerTable[BACKGROUND_DISPLAY_LAYER_0]);
    LayerConfig(&LayerTable[FOREGROUND_DISPLAY_LAYER_0]);
}

//-------------------------------------------------------------------------------------------------
// Global Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   pArg
//  Return:         None
//
//  Description:    LCD configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::Initialize(void* pArg)
{
    VAR_UNUSED(pArg);
    IO_Initialize();                    // Initialize the IO on the processor
    LCD_Initialize();                   // Initialize the in processor LCD controller
    LayerInitialize();
    DisplayOn();
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DisplayOn
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Enables the display.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DisplayOn(void)
{
    LTDC->GCR |= LTDC_GCR_LTDCEN;
    IO_SetPinHigh(IO_LCD_TFT_DISPLAY);
    IO_SetPinHigh(IO_LCD_TFT_BL_CTRL);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DisplayOff
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Disables the display.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DisplayOff(void)
{
    LTDC->GCR &= ~(LTDC_GCR_LTDCEN);
    IO_SetPinLow(IO_LCD_TFT_DISPLAY);
    IO_SetPinLow(IO_LCD_TFT_BL_CTRL);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LayerConfig
//
//  Parameter(s):   CLayer* pLayer
//  Return:         None
//
//  Description:    Configuration for layer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::LayerConfig(CLayer* pLayer)
{
    uint32_t    PixelFormat;
    uint32_t    PixelSize;
    LayerType_e ActiveLayer;
    LTDC_Layer_TypeDef* pActiveLayer;

    ActiveLayer = pLayer->GetActive();
    PixelFormat = pLayer->GetPixelFormat();
    PixelSize   = GFX_PixelSize[PixelFormat];

    if(ActiveLayer < GRAFX_NUMBER_OF_ACTIVE_LAYER)
    {
        pActiveLayer = (ActiveLayer == 0) ? LTDC_Layer1 : LTDC_Layer2;
        pActiveLayer->WHPCR = ((((LTDC->BPCR & LTDC_BPCR_AHBP) >> LTDC_BPCR_AHBP_Pos) + 1) |    // Configures the horizontal start and stop position
                               (((GRAFX_SIZE_X - 1) + ((LTDC->BPCR & LTDC_BPCR_AHBP) >> LTDC_BPCR_AHBP_Pos)) << LTDC_LxWHPCR_WHSPPOS_Pos));
        pActiveLayer->WVPCR = (((LTDC->BPCR & LTDC_BPCR_AVBP) + 1) |                            // Configures the vertical start and stop position
                               (((GRAFX_SIZE_Y - 1) +  (LTDC->BPCR & LTDC_BPCR_AVBP)) << LTDC_LxWVPCR_WVSPPOS_Pos));
        pActiveLayer->PFCR  = DRV_PixelFormatTable[PixelFormat];                                // Specifies the pixel format
        pActiveLayer->DCCR  = 0;                                                                // Configures the default color values ( all zero)
        pActiveLayer->CACR  = (uint32_t)pLayer->GetAlpha();                                     // Specifies the constant alpha value
        pActiveLayer->BFCR  = (LTDC_BLENDING_FACTOR1_PAxCA | LTDC_BLENDING_FACTOR2_PAxCA);      // Specifies the blending factors
        pActiveLayer->CFBAR = pLayer->GetAddress();                                             // Configures the color frame buffer start address
        pActiveLayer->CFBLR = (((GRAFX_SIZE_X * PixelSize) << LTDC_LxCFBLR_CFBP_Pos) |          // Configures the color frame buffer pitch in byte
                               (((GRAFX_SIZE_X - 1) * PixelSize)  + 3));
        pActiveLayer->CFBLNR = GRAFX_SIZE_Y;                                                    // Configures the frame buffer line number
        SET_BIT(pActiveLayer->CR, LTDC_LxCR_LEN);                                               // Enable LTDC_Layer by setting LEN bit
        LTDC->SRCR = LTDC_SRCR_IMR;                                                             // Reload
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           BlockCopy
//
//  Parameter(s):   void*           pSrc
//                  uint16_t        X
//                  uint16_t        Y
//                  uint16_t        Width
//                  uint16_t        Height
//                  uint16_t        DstX
//                  uint16_t        DstY
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
void GrafxDriver::BlockCopy(void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t DstX, uint16_t DstY, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    Box_t Box;

    Box.Pos.X = X;
    Box.Pos.Y = Y;
    Box.Size.Width  = Width;
    Box.Size.Height = Height;

    this->BlockCopy(pSrc, &Box, &Box.Pos, SrcPixelFormat, BlendMode);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           BlockCopy
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
void GrafxDriver::BlockCopy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormatSrc     = DRV_PixelFormatTable[SrcPixelFormat];
    PixelFormatDst     = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pDstPos->Y * GRAFX_SIZE_X) + pDstPos->X) * (uint32_t)PixelSize);
    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;
    DMA2D->CR          = (BlendMode == CLEAR_BLEND) ? DMA2D_M2M : DMA2D_M2M_BLEND;                                  // Memory to memory and TCIE blending BG + Source

    //Source 1
    DMA2D->FGMAR       = (uint32_t)(pSrc) +(((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);    // Source address
    DMA2D->FGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Source line offset
    DMA2D->FGPFCCR     = PixelFormatSrc;                                                                            // Defines the size of pixel

    // Source 2
    DMA2D->BGMAR       = Address;                                                                                   // Source address
    DMA2D->BGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Source line offset
    DMA2D->BGPFCCR     = PixelFormatDst;                                                                            // Defines the size of pixel

    //Destination
    DMA2D->OMAR        = Address;                                                                                   // Destination address
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;                                       // Destination line offset
    DMA2D->OPFCCR      = PixelFormatDst;                                                                            // Defines the size of pixel

    DMA2D->NLR         = AreaConfig.u_32;                                                                           // Size configuration of area to be transfered
    SET_BIT(DMA2D->CR, DMA2D_CR_START);                                                                             // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                                                                            // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CopyLinear
//
//  Parameter(s):   void*           pSrc
//                  Box_t*          pBox
//                  PixelFormat_e   SrcPixelFormat_e)
//                  BlendMode_e     BlendMode
//  Return:         None
//
//  Description:    Copy a rectangle region from linear memory region to square memory area
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::CopyLinear(void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    uint32_t           PixelFormatSrc;
    uint32_t           PixelFormatDst;
    uint32_t           Address;
    s32_t              AreaConfig;
    CLayer*            pLayer;
    uint8_t            PixelSize;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormatSrc     = DRV_PixelFormatTable[SrcPixelFormat];
    PixelFormatDst     = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pBox->Pos.Y * GRAFX_SIZE_X) + pBox->Pos.X) * (uint32_t)PixelSize);
    AreaConfig.u_16.u1 = pBox->Size.Width;
    AreaConfig.u_16.u0 = pBox->Size.Height;

    if(PixelFormatSrc == PixelFormatDst)
    {
        DMA2D->CR = (BlendMode == CLEAR_BLEND) ? DMA2D_M2M : DMA2D_M2M_BLEND;   // Memory to memory or M2M with blending BG + Source
    }
    else
    {
        DMA2D->CR = DMA2D_M2M_PFC;                                              // Memory to memory with pixel conversion
    }

    // Source 1
    DMA2D->FGMAR   = (uint32_t)pSrc;                                            // Source address
    DMA2D->FGOR    = 0;                                                         // Source line offset none as we are linear
    DMA2D->FGPFCCR = PixelFormatSrc;                                            // Defines the size of pixel

    // Source 2 (Source2 vs Destination = Read modify write)
    DMA2D->BGMAR   = Address;                                                   // Source address
    DMA2D->BGOR    = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;       // Source line offset
    DMA2D->BGPFCCR = PixelFormatDst;                                            // Defines the size of pixel

    // Destination
    DMA2D->OMAR    = Address;                                                   // Destination address
    DMA2D->OOR     = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;       // Destination line offset
    DMA2D->OPFCCR  = PixelFormatDst;                                            // Defines the size of pixel

    DMA2D->NLR     = AreaConfig.u_32;                                           // Size configuration of area to be transfered
    SET_BIT(DMA2D->CR, DMA2D_CR_START);                                         // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                                        // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawRectangle
//
//  Parameter(s):   Box_t*   pBox
//  Return:         None
//
//  Description:    Fill a region in a specific color
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DrawRectangle(Box_t* pBox)
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

    DMA2D->CR          = DMA2D_R2M;                                            // Register to memory
    DMA2D->OCOLR       = Color;                                                // Color to be used
    DMA2D->OMAR        = Address;                                              // Destination address
    DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)pBox->Size.Width;  // Destination line offset
    DMA2D->OPFCCR      = PixelFormat;                                          // Defines the number of pixels to be transfered
    DMA2D->NLR         = AreaConfig.u_32;                                      // Size configuration of area to be transfered
    SET_BIT(DMA2D->CR, DMA2D_CR_START);                                        // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                                       // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawBox
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
void GrafxDriver::DrawBox(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness)
{
    uint16_t X2 = PosX + Length;
    uint16_t Y2 = PosY + Height;

    DrawVLine(PosX,           PosY, Y2, Thickness);
    DrawVLine(X2 - Thickness, PosY, Y2, Thickness);
    DrawHLine(PosY,           PosX, X2, Thickness);
    DrawHLine(Y2 - Thickness, PosX, X2, Thickness);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawPixel
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
void GrafxDriver::DrawPixel(uint16_t PosX, uint16_t PosY)
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

    DMA2D->CR      = DMA2D_R2M;                                 // Register to memory
    DMA2D->OCOLR   = Color;                                     // Color to be used
    DMA2D->OMAR    = Address;                                   // Destination address
    DMA2D->OPFCCR  = PixelFormat;                               // Defines the number of pixels to be transfered
    DMA2D->NLR     = 0x00010001;                                // Size configuration of area to be transfered
    SET_BIT(DMA2D->CR, DMA2D_CR_START);                         // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                        // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawHLine
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
void GrafxDriver::DrawHLine(uint16_t PosY, uint16_t PosX1, uint16_t PosX2, uint16_t Thickness)
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

    DrawLine(PosX1, PosY, Length, Thickness, DRAW_HORIZONTAL);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawVLine
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
void GrafxDriver::DrawVLine(uint16_t PosX, uint16_t PosY1, uint16_t PosY2, uint16_t Thickness)
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

    DrawLine(PosX, PosY1, Length, Thickness, DRAW_VERTICAL);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawLine
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
void GrafxDriver::DrawLine(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction)
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

    DMA2D->CR      = DMA2D_R2M;                                             // Register to memory
    DMA2D->OCOLR   = Color;                                                 // Color to be used
    DMA2D->OMAR    = Address;                                               // Destination address
    DMA2D->OOR     = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1; // Destination line offset
    DMA2D->OPFCCR  = PixelFormat;                                           // Defines the number of pixels to be transfered
    DMA2D->NLR     = AreaConfig.u_32;                                       // Size configuration of area to be transfered
    SET_BIT(DMA2D->CR, DMA2D_CR_START);                                     // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                                    // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           PrintFont
//
//  Parameter(s):   FONT_sDescriptor*   pDescriptor
//                  Cartesian_t*        pPos
//  Return:         none
//
//  Description:    This function will print a font to drawing layer with the drawing color
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::PrintFont(FontDescriptor_t* pDescriptor, Cartesian_t* pPos)
{
    uint8_t            PixelSize;
    uint32_t           Address;
    CLayer*            pLayer;
    uint32_t           PixelFormat;
    s32_t              AreaConfig;

    pLayer             = &LayerTable[CLayer::GetDrawing()];
    PixelFormat        = DRV_PixelFormatTable[pLayer->GetPixelFormat()];
    PixelSize          = pLayer->GetPixelSize();
    Address            = pLayer->GetAddress() + (((pPos->Y * GRAFX_SIZE_X) + pPos->X) * (uint32_t)PixelSize);
    AreaConfig.u_16.u1 = pDescriptor->Size.Width;
    AreaConfig.u_16.u0 = pDescriptor->Size.Height;

    DMA2D->CR = DMA2D_M2M_BLEND;                                                // Memory to memory blending BG + Source

    // Font layer in Alpha blending linear (A8)
    DMA2D->FGMAR   = (uint32_t)pDescriptor->pAddress;                           // Source address 1
    DMA2D->FGOR    = 0;                                                         // Font source line offset - none as we are linear
    DMA2D->FGCOLR  = pLayer->GetTextColor();
    DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_A8;                                      // Defines the number of pixels to be transfered

    DMA2D->BGMAR   = Address;                                                   // Source address 2
    DMA2D->BGOR    = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Font source line offset - none as we are linear
    DMA2D->BGPFCCR = PixelFormat;                                               // Defines the number of pixels to be transfered

    // Output Layer
    DMA2D->OMAR    = Address;
    DMA2D->OOR     = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Destination line offset
    DMA2D->OPFCCR  = PixelFormat;

    // Area
    DMA2D->NLR     = AreaConfig.u_32;                                           // Size configuration of area to be transfered

    SET_BIT(DMA2D->CR, DMA2D_CR_START);                                         // Start operation
    while(DMA2D->CR & DMA2D_CR_START){};                                        // Wait until transfer is done
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           WaitFor_V_Sync
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
void GrafxDriver::WaitFor_V_Sync(void)
{
    // Wait for Vertical sync to occur
    while((LTDC->CDSR & LTDC_CDSR_VSYNCS) == 0);
}
#endif

// implemente line irq for vsync... avec transfert

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
