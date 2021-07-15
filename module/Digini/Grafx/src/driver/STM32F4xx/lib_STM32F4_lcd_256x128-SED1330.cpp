//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_256x128-SED1330.cpp
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
#include "stm32f4xx.h"
#include "lib_grafx.h"
#include "bsp.h"

uint8_t g_LayerDefinition;


void SED1330_Cursor(uint8_t byType, uint8_t byBehavior);
void SED1330_GotoMemory(int nAddress);

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

        #define SED1330_RD                    p3_3
        #define SED1330_WR                     p3_4
        #define SED1330_A0                  p3_5
        #define SED1330_RESET                p3_4
        #define SED1330_READ_PORT                p10
        #define SED1330_WRITE_PORT              p10
        #define SED1330_DATA                1
        #define SED1330_COMMAND                0
        #define SED1330_PARAMETER            0
        #define SED1330_ENABLE                0
        #define SED1330_DISABLE             1

        #define SED1330_X_SIZE              256
        #define SED1330_Y_SIZE              128


//-------------------------------------------------------------------------------------------------
// Forward Declaration
//-------------------------------------------------------------------------------------------------

//static void     DRV_Line                       (uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t ThickNess, DrawMode_e Direction);
static void     DRV_IO_Initialize                   (void);     // Initialize the IO on the processor
static void     DRV_LCD_ControllerInitialize        (void);     // LCD is equipped with a IL9341 controller
static void     DRV_LTDC_Initialize                 (void);     // Initialize the in processor LCD controller
static void     DRV_LAYER_Initialize                (void);
static void     DRV_WriteCommand                    (uint8_t Register);
static void     DRV_u8_Write                        (uint8_t Data);
static void     DRV_u16_Write                       (uint16_t Data);
static void     DRV__DrawRectangle                  (Box_t* pBox, uint8_t Mode);


void         DRV_Clear            (void);


// from c file

uint8_t     SED1330_ReadByte        (void);
uint16_t     SED1330_ReadWord        (void);
void         SED1330_Pixel            (uint8_t X, uint8_t Y, uint16_t Color);
void         SED1330_PutMaskByte        (uint8_t X, uint8_t Y, uint8_t Data);
void         SED1330_PutMaskWord        (uint8_t X, uint8_t Y, uint16_t Data);
void         SED1330_Put_u8            (uint8_t X, uint8_t Y, uint8_t Data);
void         SED1330_Put_u16            (uint8_t X, uint8_t Y, uint16_t Data);

//-------------------------------------------------------------------------------------------------
// private const(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// private variable(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_IO_Initialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    GPIO configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_IO_Initialize(void)
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
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,  9); //GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,  9);//GPIO_AF_LTDC);
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
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

    // GPIOG configuration
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource10,  9);//GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11,  GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12,  9);//GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStruct);

    // The following pin are output
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

    // Port C pin 2 is a simple output to control CS on SPI interface
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);

    // Port D pin 13 is a simple output to control C/D pin on LCD
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_LCD_ControllerInitialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    LCD is equipped with a IL9341 controller that need to be initialize
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_LCD_ControllerInitialize(void)
{
    SED1330_RD = SED1330_DISABLE;
    SED1330_WR = SED1330_DISABLE;

    // System Control
    DRV_WriteCommand(SED1330_SYSTEM_SET);
    DRV_u8_Write(0x30);                //P1    IV=1 W/S=0 M2=0 M1=0 M0=0
    DRV_u8_Write(0x87);                //P2    W/F=1 FX=7
    DRV_u8_Write(0x07);                //P3    FY=7
    DRV_u8_Write(0x1F);                //P4    C/R=(32)-1 = 0x1F
    DRV_u8_Write(0x24);                //P5    T/CR=C/R+4 = 0x24
    DRV_u8_Write(0x7F);                //P6    Height in Line Of LCD
    DRV_u8_Write(SED1330_LINE_LENGTH);    //P7    Address Range virtual Screen (LCD = 42,Virtual = 48)
    DRV_u8_Write(0x00);                //P8

    // Set Device memory Map
    DRV_WriteCommand(SED1330_SCROLL);
    DRV_u8_Write(SED1330_TEXT_LOW);    //P1    Text Area Layer 1 Start Address
    DRV_u8_Write(SED1330_TEXT_HIGH);    //P2    SAD1 = 0x0000
    DRV_u8_Write(0x80);                //P3    SL1  = 0x80
    DRV_u8_Write(SED1330_GRAPH_LOW);    //P4    Graphic Area Layer 2 Start Address
    DRV_u8_Write(SED1330_GRAPH_HIGH);    //P5    SAD2 = 0x0800
    DRV_u8_Write(0x80);                //P6    SL2  = 0x80
    DRV_u8_Write(SED1330_TEXT_LOW);    //P7    Text Area Layer 1 Start Address
    DRV_u8_Write(SED1330_TEXT_HIGH);    //P8    SAD2 = 0x0000

    // Set CG Ram Address
    DRV_WriteCommand(SED1330_CARGEN_ADDRESS);
    DRV_u8_Write(SED1330_CGRAM_LOW);            //P1    Address Set To 0x300
    DRV_u8_Write(SED1330_CGRAM_HIGH);            //P2

    // Set Horizontal Shift
    DRV_WriteCommand(SED1330_SET_HORIZONTAL_SCROLL);
    DRV_u8_Write(0x00);                //P1    No Horizontal Pixel Shift

    // Set Layer Behavior
    DRV_WriteCommand(SED1330_OVERLAY);
//    DRV_u8_Write(TWO_LAYER | TEXT_MODE | OR_LOGIC);                                                //P1
    DRV_u8_Write(SED1330_TWO_LAYER | SED1330_OR_LOGIC);                                                //P1

    DRV_WriteCommand(SED1330_DISPLAY_OFF);
    g_LayerDefinition = SED1330_CURSOR_BLANK | SED1330_BLOCK_1_STEADY | SED1330_BLOCK_2_STEADY | SED1330_BLOCK_3_STEADY;
    DRV_u8_Write(g_LayerDefinition);    //P1

    // Clear display
    DRV_Clear();

    // Set cursor form and put Display ON
    DRV_SetCursor(SED1330_BLOCK, SED1330_CURSOR_BLANK);
    DRV_WriteCommand(SED1330_CURSOR_RIGHT);
}


//-------------------------------------------------------------------------------------------------
//
//  Function name:  DRV_LayerConfig
//
//  Parameter(s):   CLayer* pLayer
//  Return:         None
//
//  Description:    Configuration for layer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_LAYER_Initialize(void)
{
//    DRV_LayerConfig(&LayerTable[BACKGROUND_DISPLAY_LAYER_0]);
//    DRV_LayerConfig(&LayerTable[FOREGROUND_DISPLAY_LAYER_0]);
}


//-------------------------------------------------------------------------------------------------
//
//  Function name:  WriteCommand
//
//  Parameter(s):   uint8_t         Command
//  Return value:   None
//
//  Description:    Send a command to LCD
//
//  Note(s):        any sub data will be sent using LCD_WriteByte
//
//-------------------------------------------------------------------------------------------------
static void DRV_WriteCommand(uint8_t Command)
{
       SED1330_A0 = SED1330_COMMAND;
    DRV_u8_Write(Command);
    SED1330_A0 = SED1330_DATA;            // There is almost all the time DATA following Command
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: DRV_u8_Read
//
//   Parameter(s):  None
//   Return value:  uint8_t
//
//   Description:   Read 8 bits data from LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint8_t DRV_u8_Read()
{
    uint8_t Data;

    SED1330_RD = SED1330_ENABLE;
    DelayUs(1);
    Data = SED1330_READ_PORT;
    SED1330_RD = SED1330_DISABLE;
    return Data;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: DRV_u16_Read
//
//   Parameter(s):  None
//   Return value:  uint16_t        Data
//
//   Description:   Read 15 bits data from LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
WORD DRV_u16_Read()
{
    //????? don't know if this work   -> speculation
    uint16_t  Data;

    SED1330_RD = SED1330_ENABLE;
    DelayUs(1);
    Data = SED1330_READ_PORT << 8;
    SED1330_RD = SED1330_DISABLE;
    SED1330_RD = SED1330_ENABLE;
    DelayUs(1);
    Data |= SED1330_READ_PORT;
    SED1330_RD = SED1330_DISABLE;
    return Data;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: DRV_u8_Write
//
//   Parameter(s):  uint8_t         Data
//   Return value:  None
//
//   Description:   Write 8 bits data to LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_u8_Write(uint8_t Data)
{
    SED1330_WRITE_PORT = Data;
    SED1330_WR = SED1330_ENABLE;
    DelayUs(1);
    SED1330_WR = SED1330_DISABLE;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: DRV_u16_Write
//
//   Parameter(s):  uint16_t         Data
//   Return value:  None
//
//   Description:   Write 16 bits data to LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV_u16_Write(uint16_t Data)
{
    SED1330_WRITE_PORT = (uint8_t)(Data >> 8);
    SED1330_WR = SED1330_ENABLE;
    DelayUs(1);
    SED1330_WR = SED1330_DISABLE;
    SED1330_WRITE_PORT = (uint8_t)Data;
    SED1330_WR = SED1330_ENABLE;
    DelayUs(1);
}

//-----------------------------------------------------------------------------
//
// Set cursor form and behavior
//
//-----------------------------------------------------------------------------
void SED1330_SetCursor(uint8_t Type, uint8_t Behavior)
{
    DRV_WriteCommand(SED1330_CURSOR_FORM);
    DRV_u8_Write(0x07);                            // P1            Width Of Cursor
    DRV_u8_Write(0x07 | Type);                    // P2            Height Of Cursor + Cursor Mode

    DRV_WriteCommand(SED1330_DISPLAY_ON);
    g_LayerDefinition = (g_LayerDefinition & 0xFC) | Behavior;
    DRV_u8_Write(g_LayerDefinition);            // P1
}


//-----------------------------------------------------------------------------
//
// Goto a specific Memory Location
//
//-----------------------------------------------------------------------------
static void DRV_GotoMemory(int Address)
{
    DRV_WriteCommand(SED1330_SET_CURSOR_ADDRESS);
    DRV_u8_Write((uint8_t)Address);                    //P1
    DRV_u8_Write((uint8_t)(Address >> 8));            //P2
}


//-----------------------------------------------------------------------------
//
// Put a uint8_t in graphic memory
//
//-----------------------------------------------------------------------------
static void SED1330_PutByte(int Address, uint8_t Byte)
{
    DRV_GotoMemory(SED1330_GRAPHIC_MEMORY + Address);
    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    DRV_u8_Write(Byte);
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: DRV__DrawRectangle
//
//   Parameter(s):  Box_t*  pBox
//                  uint8_t Mode
//   Return value:  None
//
//   Description:   Draw a box or a rectangle
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
static void DRV__DrawRectangle(Box_t* pBox, uint8_t Mode)
{
}

//-------------------------------------------------------------------------------------------------
// Global Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           Initialize
//
//  Parameter(s):   void* Arg           ( to pass any kind data the driver might need
//  Return:         None
//
//  Description:    LCD configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------

void DRV_Initialize(void* pArg)
{
    VAR_UNUSED(pArg);
    DRV_IO_Initialize();                    // Initialize the IO on the processor
    DRV_LCD_ControllerInitialize();
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
}


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
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_GotoXY
//
//  Parameter(s):   uint8_t         PosX
//                  uint8_t         PosY
//  Return:         None
//
//  Description:    Goto a specific X,Y position
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_GotoXY(uint8_t PosX, uint8_t PosY)
{
    int Offset;

    DRV_WriteCommand(SED1330_SET_CURSOR_ADDRESS);
    Offset = (SED1330_LINE_LENGTH * PosY) + PosX;
    DRV_u8_Write((uint8_t)Offset);                        // P1
    DRV_u8_Write((uint8_t)(Offset >> 8));                // P2
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
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawCircle
//
//  Parameter(s):   Circle_t*   pBox
//  Return:         None
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t Mode)
{
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
    DRV__DrawRectangle(pBox, FILL);
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
    Box_t Box;
    int   i;

    Box. = PosX;
    Box. = PosY;
    Box. = Length;
    Box. = Height;

    while(1)
    {
        DRV__DrawRectangle(pBox, NO_FILL);

        Thickness--;
        if(Thickness == 0)
        {
            return;
        }

        Box. ++;
        Box. ++;
        Box. -= 2;
        Box. -= 2;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_DrawPixel
//
//  Parameter(s):   uint16_t    PosX
//                  uint16_t    PosY
//  Return:         None
//
//  Description:    Put a pixel on LCD
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_DrawPixel(uint16_t PosX, uint16_t PosY)
{
//void SED1330_Pixel(BYTE byPosX, BYTE byPosY, BYTE byMode)
    int nOffset;
    uint8_t Byte;

    nOffset = (SED1330_LINE_LENGTH * PosY) + (PosX >> 3);
    PosX = 7 - (PosX & 0x07);

    DRV_GotoMemory(nOffset + SED1330_GRAPHIC_MEMORY);
    DRV_WriteCommand(SED1330_MEMORY_READ);
    Byte = SED1330_ReadByte();

    switch(Mode)
    {
        case ON:    { SetBitByte(Byte, PosX); break; }
        case OFF:   { ClrBitByte(Byte, PosX); break; }
        case TOGGLE:{ ToggleBitByte(Byte, PosX); break; }
    }

    DRV_GotoMemory(nOffset + SED1330_GRAPHIC_MEMORY);
    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    DRV_u8_Write(Byte);

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
    //pLayer             = &LayerTable[CLayer::GetDrawing()];
    //PixFmt             = pLayer->GetPixelFormat();
    //PixelFormat        = DRV_PixelFormatTable[PixFmt];
    //PixelSize          = pLayer->GetPixelSize();
    //Address            = pLayer->GetAddress() + (((pPos->Y * GRAFX_SIZE_X) + pPos->X) * (uint32_t)PixelSize);
    //AreaConfig.u_16.u1 = pDescriptor->Size.Width;
    //AreaConfig.u_16.u0 = pDescriptor->Size.Height;

//    DMA2D->FGMAR       = (uint32_t)pDescriptor->pAddress;                           // Source address 1
//    DMA2D->FGCOLR      = pLayer->GetTextColor();
//    DMA2D->BGOR        = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Font source line offset - none as we are linear
    //DMA2D->BGPFCCR     = PixelFormat;                                               // Defines the number of pixels to be transfered

    // Output Layer
//    DMA2D->OMAR        = Address;
    //DMA2D->OOR         = (uint32_t)GRAFX_SIZE_X - (uint32_t)AreaConfig.u_16.u1;     // Destination line offset
    //DMA2D->OPFCCR      = PixelFormat;

    // Area
    //DMA2D->NLR         = AreaConfig.u_32;                                           // Size configuration of area to be transfered
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
void DRV_DisplayOn(void)
{
    DRV_WriteCommand(DRV_LCD_DISPLAY_ON);
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
void DRV_DisplayOff(void)
{
    DRV_WriteCommand(DRV_LCD_DISPLAY_OFF);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DRV_Clear
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Clear and home the LCD
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DRV_Clear(void)
{
    int i;

    // Clear Text Area
    DRV_GotoMemory(SED1330_TEXT_MEMORY);
    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    for(i = 0; i < SED1330_TEXT_SIZE; i++)
    {
        DRV_u8_Write(ASCII_SPACE);
    }

    // Clear Graphic Area
    DRV_GotoMemory(SED1330_GRAPHIC_MEMORY);
    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    for(i = 0; i < SED1330_GRAPHIC_SIZE; i++)
    {
        DRV_u8_Write(0x00);
    }
}



//-----------------------------------------------------------------------------
//
// Transfert bitmap for a custom character
//
//-----------------------------------------------------------------------------
void DRV_SetCustomFont(uint8_t Number, const char* pString)
{
    uint8_t Count = 8;

    DRV_WriteCommand(SED1330_SET_CURSOR_ADDRESS);
    Number &= 0x3F;                                // no value over 63
    DRV_u8_Write(Number << 8);                    // P1    ??????????  envoie 0 alors le clown!!
    DRV_u8_Write(Number >> 5);                    // P2

    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    do
    {
        Count--;
        DRV_u8_Write(pString[Count]);
    }
    while(Count != 0);
}


//-----------------------------------------------------------------------------
//
// write a string of chars to the LCD
//
//-----------------------------------------------------------------------------
void DRV_Puts(const char* pString)
{
    uint8_t i;

    i = 0;
    DRV_WriteCommand(SED1330_MEMORY_WRITE);

    while(sString[i] != ASCII_NULL)
    {
        DRV_u8_Write(pString[i++]);
    }
}

//-----------------------------------------------------------------------------
//
// write one character to the LCD
//
//-----------------------------------------------------------------------------
void DRV_Putc(char Character)
{
    DRV_WriteCommand(SED1330_MEMORY_WRITE);
    DRV_u8_Write(Character);
}


//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

