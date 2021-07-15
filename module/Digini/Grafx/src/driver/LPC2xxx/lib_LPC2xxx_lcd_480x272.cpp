//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_LPC2xxx_lcd_480x272.cpp
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
#ifdef DIGINI_USE_DRV_LPC2XXX_480X272

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f4xx.h"
#include "lib_class_LPC2XXX_lcd_480X272.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           GRAFX_DRV_Config
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    GPIO configuration specific for the LCD and processor used by this driver
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void GRAFX_DRV_Config()
{
    // TODO here initialize the IO and LCD control pin

    // Enable  Clocks

    // +------------------------+-----------------------+----------------------------+
    // +                       LCD pins assignment                                   +
    // +------------------------+-----------------------+----------------------------+
    // |  LCD_TFT R0 <->        |  LCD_TFT G0 <->       |  LCD_TFT B0 <->            |
    // |  LCD_TFT R1 <->        |  LCD_TFT G1 <->       |  LCD_TFT B1 <->            |
    // |  LCD_TFT R2 <->        |  LCD_TFT G2 <->       |  LCD_TFT B2 <->            |
    // |  LCD_TFT R3 <->        |  LCD_TFT G3 <->       |  LCD_TFT B3 <->            |
    // |  LCD_TFT R4 <->        |  LCD_TFT G4 <->       |  LCD_TFT B4 <->            |
    // |  LCD_TFT R5 <->        |  LCD_TFT G5 <->       |  LCD_TFT B5 <->            |
    // |  LCD_TFT R6 <->        |  LCD_TFT G6 <->       |  LCD_TFT B6 <->            |
    // |  LCD_TFT R7 <->        |  LCD_TFT G7 <->       |  LCD_TFT B7 <->            |
    // -------------------------------------------------------------------------------
    // |  LCD_TFT HSYNC <->        | LCDTFT VSYNC <->        |
    // |  LCD_TFT CLK   <->        | LCD_TFT DE   <->        |
    // -------------------------------------------------------
    // |  LCD_BL_CTRL   <->        |
    // -----------------------------

    // ---- GPIOs Configuration ----

    // Init GLCD controller
    PCONP_bit.PCLCD             = 1; // enable LCD controller clock
    CRSR_CTRL_bit.CrsrOn        = 0; // Disable cursor
    LCD_CTRL_bit.LcdEn          = 0; // disable GLCD controller
    LCD_CTRL_bit.LcdBpp         = 5; // 24 bpp
    LCD_CTRL_bit.LcdTFT         = 1; // TFT panel
    LCD_CTRL_bit.LcdDual        = 0; // single panel
    LCD_CTRL_bit.BGR            = 0; // normal output
    LCD_CTRL_bit.BEBO           = 0; // little endian byte order
    LCD_CTRL_bit.BEPO           = 0; // little endian pix order
    LCD_CTRL_bit.LcdPwr         = 0; // disable power

    // init pixel clock
    LCD_CFG_bit.CLKDIV  = SYS_GetFsclk() / (Int32U)
    C_GLCD_PIX_CLK;
    LCD_POL_bit.BCD             = 1; //  bypass internal clk divider
    LCD_POL_bit.CLKSEL          = 0; //  clock source for the LCD block is HCLK
    LCD_POL_bit.IVS             = 1; //  LCDFP pin is active LOW and inactive HIGH
    LCD_POL_bit.IHS             = 1; // LCDLP pin is active LOW and inactive HIGH
    LCD_POL_bit.IPC             = 1; // data is driven out into the LCD on the falling edge
    LCD_POL_bit.IOE             = 0; // active high
    LCD_POL_bit.CPL             = GRAFX_SIZE_X-1;// init Horizontal Timing
    LCD_TIMH_bit.HBP            = GRAFX_HBP - 1;
    LCD_TIMH_bit.HFP            = GRAFX_HFP - 1;
    LCD_TIMH_bit.HSW            = C_GLCD_H_PULSE - 1;
    LCD_TIMH_bit.PPL            = (GRAFX_SIZE_X / 16) - 1;

    // init Vertical Timing
    LCD_TIMV_bit.VBP            = GRAFX_VBP;
    LCD_TIMV_bit.VFP            = GRAFX_VFP;
    LCD_TIMV_bit.VSW            = C_GLCD_V_PULSE;
    LCD_TIMV_bit.LPP            = GRAFX_SIZE_Y - 1;
}


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_DRV_LPC2XXX_480X272
#endif // DIGINI_USE_GRAFX
