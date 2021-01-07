//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_common.cpp
//
//-------------------------------------------------------------------------------------------------

#ifdef DIGINI_USE_GRAFX
#ifdef STM32F429_439xx

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_lcd_driver.h"
#include "lib_grafx.h"
#include "stm32f4xx_include.h"

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

    DRV_Config();                                                                             // Configure the LCD Control pins

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
    eLayerType                  ActiveLayer;

    ActiveLayer = pLayer->GetLayer();

    if(ActiveLayer < GRAFX_NUMBER_OF_ACTIVE_LAYER)
    {
        pLTDC_SelectedLayer = (LTDC_Layer_TypeDef*)LTDC_baLayer[ActiveLayer];
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
        LTDC_Layer_InitStruct.LTDC_PixelFormat       = PixelFormatTable[pLayer->GetPixelFormat()];          // Pixel Format configuration
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
    while(LTDC_GetCDStatus(LTDC_CDSR_VSYNCS) != SET);
}


#endif // STM32F429_439xx
#endif // DIGINI_USE_GRAFX
