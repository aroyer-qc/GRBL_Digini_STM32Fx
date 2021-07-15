//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F4_lcd_128x128-SSD1779.cpp
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
#define LIB_SSD1779_GLOBAL
#include "lib_STM32F4_lcd_128x128-SSD1779.h"
#undef  LIB_SSD1779_GLOBAL
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SSD1779_SET_COLUMN_ADDRESS          0x15
#define SSD1779_SET_POWER_CONTROL           0x20
#define SSD1779_SET_FIRST_DISPLAY           0x44
#define SSD1779_WRITE_DISPLAY_DATA_MODE     0x5C
#define SSD1779_READ_DISPLAY_DATA_MODE      0x5D            // WriteCommand then read data
#define SSD1779_READ_STATUS                 0x5D            // ReadCommand  D/C must stay low to read status
#define SSD1779_SET_PAGE_ADDRESS            0x75
#define SSD1779_SET_CONTRAST_LEVEL          0x81
#define SSD1779_SET_TEMP_COMPENSATION       0x82
#define SSD1779_SLEEP                       0x94
#define SSD1779_SET_INVERSE_DISPLAY         0xA6
#define SSD1779_ENTER_PARTIAL_DISPLAY       0xA8
#define SSD1779_EXIT_PARTIAL_DISPLAY        0xA9
#define SSD1779_SET_AREA_SCROLL             0xAA
#define SSD1779_SET_SCROLL_START            0xAB
#define SSD1779_DISPLAY                     0xAE
#define SSD1779_SET_COM_OUTPUT_SCAN         0xBB
#define SSD1779_SET_SCAN_RGB_ARRANGEMENT    0xBC
#define SSD1779_SET_DISPLAY_CONTROL         0xCA
#define SSD1779_CONTRAST_LEVEL              0xD6
#define SSD1779_INTERNAL_OSC                0xD0
#define SSD1779_SET_READ_MODIFY_WRITE_MODE  0xE0
#define SSD1779_EXIT_READ_MODIFY_WRITE_MODE 0xEE
#define SSD1779_SET_COM_SEQUENCE            0xF1
#define SSD1779_STABILIZED_VOLT_GENERATOR   0xF3
#define SSD1779_BIAS_RATIO                  0xFB

// graphic command
#define SSD1779_DRAW_LINE                   0x83
#define SSD1779_FILL_MODE                   0x92
#define SSD1779_DRAW_RECTANGLE              0x84
#define SSD1779_DRAW_CIRCLE                 0x86
#define SSD1779_COPY                        0x8A
#define SSD1779_DIM_WINDOW                  0x8C
#define SSD1779_CLEAR_WINDOW                0x8E

//  use ON or OFF on this Command
#define SSD1779_Display(State)              WriteCommand(SSD1779_DISPLAY | State)

//  use OSC_ON or OSC_OFF on this Command
#define SSD1779_Oscillator(State)           WriteCommand(SSD1779_INTERNAL_OSC | State)

//  use ENTER_MODE or EXIT_MODE on this Command
#define SSD1779_SleepMode(State)            WriteCommand(SSD1779_SLEEP | State)


#define SSD1779_Reverse(s)                  WriteCommand(SSD1779_SET_INVERSE_DISPLAY | s)

//  use a value between 0 and 64 on this command
#define SSD1779_SetContrast(Level)          {                                             \
                                                WriteCommand(SSD1779_SET_CONTRAST_LEVEL); \
                                                WriteData(uint8_t(Level));                \
                                                WriteData(uint8_t(0x05));                 \
                                            }
//LCD_Contrast = Level;

//#define ENTER_MODE            1
#define SSD1779_EXIT_MODE       0
//#define DECREASE              1
//#define INCREASE              0
#define SSD1779_OSC_ON          0x01
//#define OSC_OFF               0x02

#define SSD1779_OFF             0
#define SSD1779_ON              1

//-------------------------------------------------------------------------------------------------
// Private Function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           ControllerInitialize
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    LCD is equipped with a IL9341 controller that need to be initialize
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::ControllerInitialize(void)
{
    uint8_t Status;
    uint16_t Data;

    IO_PinInit(IO_SSD1779_DC);
    IO_PinInit(IO_SSD1779_RESET);
    //IO_PinInit(IO_SSD1779_BACK_CTRL);
    m_Bus.Initialize(SSD1779_BUS);
    for(int i = 0; i < 10000; i++) { __asm("nop"); }            // Hold Reset at least 10 uSec
    IO_SetPinHigh(IO_SSD1779_RESET);

    SSD1779_Oscillator(SSD1779_OSC_ON);                         // Enable Oscillator
    SSD1779_SleepMode(SSD1779_EXIT_MODE);                       // Exit Sleep mode
    WriteCommand(SSD1779_SET_POWER_CONTROL);                    // Turn on the reference voltage generator, internal regulator and voltage follower. Select booster level.
    WriteData(uint8_t(0x07));

    WriteCommand(SSD1779_STABILIZED_VOLT_GENERATOR);            // Stabilize the voltage regulator
    WriteData(uint16_t(0x9005));
    WriteData(uint16_t(0x6084));

    WriteCommand(SSD1779_BIAS_RATIO);
    WriteData(uint8_t(0x02));                                   // 1/9

    SSD1779_SetContrast(20);

    WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
    WriteData(uint8_t(0));
    WriteData(uint8_t(127));

    WriteCommand(SSD1779_SET_PAGE_ADDRESS);
    WriteData(uint8_t(0));
    WriteData(uint8_t(127));

    WriteCommand(SSD1779_SET_DISPLAY_CONTROL);
    WriteData(uint16_t(0x0020));
    WriteData(uint8_t(0) /*DUMMY*/);

    WriteCommand(SSD1779_SET_SCAN_RGB_ARRANGEMENT);
    WriteData(uint16_t(0));
    WriteData(uint8_t(0));

    WriteCommand(SSD1779_SET_COM_OUTPUT_SCAN);
    WriteData(uint8_t(2));

    WriteCommand(SSD1779_SET_COM_SEQUENCE);
    WriteData(uint8_t(0xF0));
    WriteData(uint8_t(0));

    SSD1779_Reverse(SSD1779_ON);
    WriteCommand(SSD1779_EXIT_READ_MODIFY_WRITE_MODE);

    SSD1779_Display(SSD1779_ON);

    Status = ReadCommand(SSD1779_READ_STATUS);

  //  WriteCommand(SSD1779_SET_PAGE_ADDRESS);
  //  WriteData(uint16_t(20));
  //  WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
  //  WriteData(uint16_t(20));

  //  WriteCommand(SSD1779_WRITE_DISPLAY_DATA_MODE);
  //  WriteData(uint16_t(0x55AA));

  //  WriteCommand(SSD1779_SET_PAGE_ADDRESS);
  //  WriteData(uint16_t(40));
  //  WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
  //  WriteData(uint16_t(40));

  //  WriteData(uint16_t(0xAA55));

  //  WriteCommand(SSD1779_SET_PAGE_ADDRESS);
  //  WriteData(uint16_t(20));
  //  WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
  //  WriteData(uint16_t(20));

  //  WriteCommand(SSD1779_READ_DISPLAY_DATA_MODE);
  //  Data = ReadData_16();

    WriteCommand(SSD1779_DRAW_CIRCLE);
    WriteData(uint8_t(64));
    WriteData(uint8_t(64));
    WriteData(uint8_t(20));
    WriteData(uint8_t(0xAA));
    WriteData(uint8_t(0x55));
    WriteData(uint8_t(0xAA));
    WriteData(uint8_t(0x55));

SSD1779_SetContrast(0);
SSD1779_SetContrast(5);
SSD1779_SetContrast(10);
SSD1779_SetContrast(15);
SSD1779_SetContrast(20);
SSD1779_SetContrast(25);
SSD1779_SetContrast(30);
SSD1779_SetContrast(35);
SSD1779_SetContrast(40);
SSD1779_SetContrast(45);
SSD1779_SetContrast(50);
SSD1779_SetContrast(55);
SSD1779_SetContrast(60);
SSD1779_SetContrast(63);

    Clear();


//    IO_SetPinLow(IO_SSD1779_BACK_CTRL);
}

//-------------------------------------------------------------------------------------------------
//
//  Function name:   WriteCommand
//
//  Parameter(s):   uint8_t         Command
//  Return value:   None
//
//  Description:    Send a command to LCD
//
//  Note(s):        any sub data will be sent using LCD_WriteByte
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::WriteCommand(uint8_t Command)
{
    IO_SetPinLow(IO_SSD1779_DC);
    m_Bus.Write(Command);
    IO_SetPinHigh(IO_SSD1779_DC);
}

//-------------------------------------------------------------------------------------------------
//
//  Function name:   ReadCommand
//
//  Parameter(s):   uint8_t         Command
//  Return value:   uint8_t         Data
//
//  Description:    Send a read command to LCD
//
//  Note(s):        any sub data will be sent using LCD_WriteByte
//
//-------------------------------------------------------------------------------------------------
uint8_t GrafxDriver::ReadCommand(uint8_t Command)
{
    uint8_t Data;

    IO_SetPinLow(IO_SSD1779_DC);
    m_Bus.Write(Command);
    Data = uint8_t(m_Bus.Read());
    IO_SetPinHigh(IO_SSD1779_DC);
    return Data;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: WriteData
//
//   Parameter(s):  uint8_t         Data
//   Return value:  None
//
//   Description:   Write 8 bits data to LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::WriteData(uint8_t Data)
{
    m_Bus.Write(Data);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: WriteData
//
//   Parameter(s):  uint16_t         Data
//   Return value:  None
//
//   Description:   Write 16 bits data to LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::WriteData(uint16_t Data)
{
    m_Bus.Write(Data >> 8);
    m_Bus.Write(Data & 0xFF);
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ReadData_8
//
//   Parameter(s):  None
//   Return value:  uint8_t         Data
//
//   Description:   Read 8 bits data from LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint8_t GrafxDriver::ReadData_8(void)
{
    return uint8_t(m_Bus.Read());
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: ReadData_16
//
//   Parameter(s):  None
//   Return value:  uint16_t         Data
//
//   Description:   Read 16 bits data from LCD
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t GrafxDriver::ReadData_16(void)
{
    uint16_t Data;

    Data  = m_Bus.Read() << 8;
    Data |= m_Bus.Read() & 0xFF;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: DrawRectangle
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
void GrafxDriver::DrawRectangle(Box_t* pBox, uint8_t Mode)
{
    m_pLayer = &LayerTable[CLayer::GetDrawing()];

    if(Mode == SSD1779_FILL_MODE)
    {
        WriteCommand(SSD1779_FILL_MODE);
        WriteData(uint8_t(0x01));
    }

    WriteCommand(SSD1779_DRAW_RECTANGLE);
    WriteData(uint8_t(pBox->Pos.X));
    WriteData(uint8_t(pBox->Pos.Y));
    WriteData(uint8_t(pBox->Pos.X + pBox->Size.Width));
    WriteData(uint8_t(pBox->Pos.Y + pBox->Size.Height));
    WriteData(uint16_t(m_pLayer->GetColor()));
    WriteData(uint16_t(m_pLayer->GetColor()));

    if(Mode == SSD1779_FILL_MODE)
    {
        WriteCommand(SSD1779_FILL_MODE);
        WriteData(uint8_t(0x00));
    }
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

void GrafxDriver::Initialize(void* pArg)
{
    VAR_UNUSED(pArg);
    ControllerInitialize();
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
    VAR_UNUSED(pLayer); // This is a single layer LCD controller
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
void GrafxDriver::Copy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat_e, BlendMode_e BlendMode)
{
/*
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
*/
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
    Box_t       Box;
    Cartesian_t Pos;

    Box.Pos.X       = X;
    Box.Pos.Y       = Y;
    Box.Size.Width  = Width;
    Box.Size.Height = Height;
    Pos.X           = DstX;
    Pos.Y           = DstY;

    this->BlockCopy(pSrc, &Box, &Pos, SrcPixelFormat, BlendMode);
}
//-------------------------------------------------------------------------------------------------
//
//  Name:           BlockCopy
//
//  Parameter(s):   void*           pSrc
//                  Box_t*          pBox
//                  Cartesian_t*    pDstPos
//                  PixelFormat_e   SrcPixelFormat
//                  BlendMode_e     BlendMode
//  Return:         None
//
//  Description:    Copy a rectangle region from 2D memory region to another 2D memory region
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::BlockCopy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    VAR_UNUSED(BlendMode);
    VAR_UNUSED(SrcPixelFormat);

    WriteCommand(SSD1779_COPY);
    WriteData(uint8_t(pBox->Pos.X));
    WriteData(uint8_t(pBox->Pos.Y));
    WriteData(uint8_t(pBox->Pos.X + pBox->Size.Width));
    WriteData(uint8_t(pBox->Pos.Y + pBox->Size.Height));
    WriteData(uint8_t(pDstPos->X));
    WriteData(uint8_t(pDstPos->Y));
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           CopyLinear
//
//  Parameter(s):   void*           pSrc
//                  Box_t*          pBox
//                  PixelFormat_e   SrcPixelFormat)
//                  BlendMode_e     BlendMode
//  Return:         None
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//  Note(s):        Source is linear
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::CopyLinear(void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
//    Not supported for now

    VAR_UNUSED(pSrc);
    VAR_UNUSED(pBox);
    VAR_UNUSED(SrcPixelFormat);
    VAR_UNUSED(BlendMode);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawCircle
//
//  Parameter(s):   Circle_t*   pBox
//  Return:         None
//
//  Description:
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t Mode)
{
    m_pLayer = &LayerTable[CLayer::GetDrawing()];

    if(Mode == SSD1779_FILL)
    {
        WriteCommand(SSD1779_FILL_MODE);
        WriteData(uint8_t(0x01));
    }

    WriteCommand(SSD1779_DRAW_CIRCLE);
    WriteData(X);
    WriteData(Y);
    WriteData(uint16_t(m_pLayer->GetColor())); // Edge
    WriteData(uint16_t(m_pLayer->GetColor())); // Fillcolor

    if(Mode == SSD1779_FILL)
    {
        WriteCommand(SSD1779_FILL_MODE);
        WriteData(uint8_t(0x00));
    }
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
    DrawRectangle(pBox, SSD1779_FILL);
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
    Box_t Box;

    Box.Pos.X = PosX;
    Box.Pos.Y = PosY;
    Box.Size.Width  = Length;
    Box.Size.Height = Height;

    while(Thickness)
    {
        DrawRectangle(&Box, SSD1779_NO_FILL);
        Box.Pos.X++;
        Box.Pos.Y++;
        Box.Size.Width  -= 2;
        Box.Size.Height -= 2;
        Thickness--;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           DrawPixel
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
void GrafxDriver::DrawPixel(uint16_t PosX, uint16_t PosY)
{
    m_pLayer = &LayerTable[CLayer::GetDrawing()];

    WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
    WriteData(uint8_t(PosX));
    WriteData(uint8_t(127));

    WriteCommand(SSD1779_SET_PAGE_ADDRESS);
    WriteData(uint8_t(PosY));
    WriteData(uint8_t(127));

    #ifdef _USE_SWAP_COLOR_
    if(LCD_Flag.bSwapColorMode == YES)
    {
        if(SSD1779_ReadWord() != LCD_SwapColor)
        {
            return;
        }

        WriteCommand(SSD1779_SET_COLUMN_ADDRESS);
        WriteData(uint8_t(PosX));
        WriteData(uint8_t(127));

        WriteCommand(SSD1779_SET_PAGE_ADDRESS);
        WriteData(uint8_t(PosY));
        WriteData(uint8_t(127));
    }
    #endif

    WriteCommand(SSD1779_WRITE_DISPLAY_DATA_MODE);
    WriteData(uint16_t(m_pLayer->GetColor()));

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
    m_pLayer = &LayerTable[CLayer::GetDrawing()];
    WriteCommand(SSD1779_DRAW_LINE);
    WriteData(uint8_t(PosX));
    WriteData(uint8_t(PosY));
    WriteData(uint8_t(PosX + Length));
    WriteData(uint8_t(PosY + ThickNess));
    WriteData(uint16_t(m_pLayer->GetColor()));
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
    s32_t         AreaConfig;
    PixelFormat_e PixelFormat;
    uint8_t       PixelSize;

    m_pLayer = &LayerTable[CLayer::GetDrawing()];

//    uint32_t           Address;

    AreaConfig.u_16.u1 = pDescriptor->Size.Width;
    AreaConfig.u_16.u0 = pDescriptor->Size.Height;
    PixelFormat        = m_pLayer->GetPixelFormat();
    PixelSize          = m_pLayer->GetPixelSize();

/*
(uint32_t)pDescriptor->pAddress;                           // Source address 1 of the font
0;                                                         // Font source line offset - none as we are linear
pLayer->GetTextColor();

//??    Address            = pLayer->GetAddress() + (((pPos->Y * GRAFX_SIZE_X) + pPos->X) * (uint32_t)PixelSize);

//must be all the info needed
*/
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DisplayOn
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Enables the Display
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DisplayOn(void)
{
    SSD1779_Display(SSD1779_ON);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           DisplayOff
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Disables the Display
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::DisplayOff(void)
{
    SSD1779_Display(SSD1779_OFF);
}



//-------------------------------------------------------------------------------------------------
//
//  Name:           Clear
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Clear and home the LCD
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GrafxDriver::Clear(void)
{
    WriteCommand(SSD1779_CLEAR_WINDOW);
    WriteData(uint8_t(0x00));                                // X1
    WriteData(uint8_t(0x00));                                // Y1
    WriteData(uint8_t(GRAFX_SIZE_X - 1));                    // X2
    WriteData(uint8_t(GRAFX_SIZE_Y - 1));                    // Y2
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX


#if 0


/*****************************************************************************

   Name:                SSD1779_PutMaskByte
   Parameter(s):        uint8_t X
                        uint8_t Y
   Return(s):           None
   Description:         display 8 consecutive pixel ON or OFF following the
                        uint8_t pattern, OFF bit stay the original color

******************************************************************************/
void SSD1779_PutMaskByte(uint8_t X, uint8_t Y, uint8_t Data)
{
    if(Data != 0)
    {
        while(Data)
        {
            if(Data & 0x80)
            {
                SSD1779_Pixel(X, Y, LCD_Color);
            }

            Data <<= 1;
            X++;
        }
    }
}

/*****************************************************************************

   Name:                SSD1779_PutMaskWord
   Parameter(s):        uint8_t X
                        uint8_t Y
   Return(s):           None
   Description:         display 8 consecutive pixel ON or OFF following the
                        uint8_t pattern, OFF bit stay the original color

******************************************************************************/
void SSD1779_PutMaskWord(uint8_t X, uint8_t Y, uint16_t Data)
{
    if(Data != 0)
    {
        while(Data)
        {
            if(Data & 0x8000)
            {
                SSD1779_Pixel(X, Y, LCD_Color);
            }

            Data <<= 1;
            X++;
        }
    }
}
/*****************************************************************************

   Name:                SSD1779_PutByte
   Parameter(s):        uint8_t X
                        uint8_t Y
                        uint8_t Data
   Return(s):           None
   Description:         display 8 consecutive pixel ON or OFF following the
                        uint8_t pattern, OFF bit stay the original color

******************************************************************************/
void SSD1779_PutByte(uint8_t X, uint8_t Y, uint8_t Data)
{
    if(Data !=0)
    {
        while(Data)
        {
            if(Data & 0x80)
            {
                SSD1779_Pixel(X, Y, LCD_Color);
            }
            else
            {
                SSD1779_Pixel(X, Y, LCD_FillColor);
            }

            Data <<= 1;
            X++;
        }
    }
}

/*****************************************************************************

   Name:                SSD1779_PutWord
   Parameter(s):        uint8_t X
                        uint8_t Y
   Return(s):           None
   Description:         display 8 consecutive pixel ON or OFF following the
                        uint8_t pattern, OFF bit stay the original color

******************************************************************************/
void SSD1779_PutWord(uint8_t X, uint8_t Y, uint16_t Data)
{
    if(Data != 0)
    {
        while(Data)
        {
            if(Data & 0x8000)
            {
                SSD1779_Pixel(X, Y, LCD_Color);
            }
            else
            {
                SSD1779_Pixel(X, Y, LCD_FillColor);
            }

            Data <<= 1;
            X++;
        }
    }
}

//-------------------------------------------------------------------------------------------------

#endif
