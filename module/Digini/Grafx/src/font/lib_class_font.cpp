//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_font.cpp
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
//
//  Notes:
//
// FONT_sStaticDescriptor inner working
//
// the high nibble bits of the first uint8_t are for the SIZE X       ___________ _______
// the low nibble bits of the first uint8_t are for the SIZE Y      |           |        Offset Y
//                                                                  |           |_______
// the high nibble bits of the second uint8_t are for the OFFSET X  |   ****    |
// the low nibble bits of the second uint8_t are for the OFFSET Y   |  **  **   |
//                                                                  |  **  **   |        Size Y
// the high nibble bits of the third uint8_t are for the PADDING X  |  **  **   |
// the low nibble bits of the third uint8_t are not used (see note) |   ****    |_______
//                                                                  |___________|
//                                                                  |  |     |  |
//                                                                  |  |     |  |_______ Padding X
//                                                                  |  |_____|__________ Size X
//                                                                  |__|________________ Offset X
//
//  Notes: Number 32 (0x20) or SPACE the third bytes low nibble hold the Font Total size in X (0 = 16)
//  Notes: Number 33 (0x21) or '!' the third bytes low nibble hold the font Height in Y (0 = 16)
//  Notes: Number 34 (0x22) or '"' the third bytes low nibble hold the font Interline
//
//
//  Raw monochrome data description
//
//  Each nibble represent a grouping of pixel
//  higher bit of grouping represent the state of the pixel
//  the lower 3 bits represent (how many pixel + 1) there is in row with this value
//
//  note that this value can be on multi line according to value on the Descriptor
//
//  |   Nibble 1    |   Nibble 2    |
//  |_______________|_______________|
//  |   |   |   |   |   |   |   |   |
//  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
//  |___|___|___|___|___|___|___|___|
//  | 0 | Number of | 0 | Number of |
//  |or | Pixel + 1 |or | Pixel + 1 |
//  | 1 |           | 1 |           |
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_typedef.h"
#include "lib_grafx.h"
#include "bsp.h"



//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

CFont        FontDefault;

//-------------------------------------------------------------------------------------------------
//
//  Name:           FONT_Initialize
//  Parameter(s):   void
//  Return:         void
//
//  Description:    This function will transfer basic font set for error and loading screen
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void FONT_Initialize(void)
{
  #if defined(DIGINI_USE_FONT_SIZE_8) || defined (DIGINI_USE_FONT_SIZE_12) || defined(DIGINI_USE_FONT_SIZE_16)
    FontDescriptor_t                FontDescriptor;
    const StaticFontDescriptor_t*   pDscFont;
    uint8_t*                        pMemory;
    uint16_t                        Counter;
    uint16_t                        SizeCount;
    uint8_t                         Font;
    uint16_t                        Character;
    uint8_t                         Value;
    uint8_t                         BitCount;
    uint8_t                         BitValue;
    FontInfo_t                      FontInfo;

    // load in RAM default 8x8 and/or 12x12 and/or 16x16 font
    for(Font = 0; Font < NB_SYSTEM_FONTS; Font++)
    {
        // Clear unused descriptor
        FontDescriptor.pAddress  = nullptr;
        FontDescriptor.TotalSize = 0;
        for(Character = 0;   Character < 32;  Character++)  DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, Font, Character);
        for(Character = 128; Character < 256; Character++)  DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, Font, Character);

        FontInfo.Width     = StaticFontDescriptor[Font][0].Padding & 0x0F;
        FontInfo.Width     = (FontInfo.Width == 0) ? 16 : FontInfo.Width;   // Save the Width
        FontInfo.Height    = StaticFontDescriptor[Font][1].Padding & 0x0F;
        FontInfo.Height    = (FontInfo.Height == 0) ? 16 : FontInfo.Height;  // Save the height of the font into database
        FontInfo.Interline = StaticFontDescriptor[Font][2].Padding & 0x0F;   // Get Interline
        DB_Central.Set(&FontInfo, GFX_FONT_INFO, Font, 0);

        Counter = 0;
        for(Character = 32; Character < 128; Character++)
        {
            DB_Central.Get(&pMemory, GFX_FREE_RAM_POINTER, 0, 0);
            pDscFont                        = &StaticFontDescriptor[Font][Character - 32];
            FontDescriptor.pAddress         = pMemory;                                                          // Set address in memory
            FontDescriptor.Size.Width       = pDscFont->Size >> 4;
            FontDescriptor.Size.Height      = pDscFont->Size & 0x0F;
            FontDescriptor.LeftBearing      = pDscFont->Offset >> 4;
            FontDescriptor.RightBearing     = pDscFont->Padding >> 4;
            FontDescriptor.OffsetY          = pDscFont->Offset & 0x0F;

            #ifdef DIGINI_USE_FONT_SIZE_16
              if(Font == SYS_FT_16)    // Only for 16x16
              {
                  if(Character > 34)         // 32,33,34 hold special value code
                  {
                      if((FontDescriptor.Size.Width == 0)  && ((pDscFont->Padding & 0x01) != 0)) FontDescriptor.Size.Width  = 16;
                      if((FontDescriptor.Size.Height == 0) && ((pDscFont->Padding & 0x02) != 0)) FontDescriptor.Size.Height = 16;
                  }
              }
            #endif

            FontDescriptor.Width      = FontDescriptor.Size.Width + FontDescriptor.LeftBearing + FontDescriptor.RightBearing;
            FontDescriptor.TotalSize  = FontDescriptor.Size.Width * FontDescriptor.Size.Height;                      // Calculate size

            #ifdef LCD_INVERT_SCREEN
              if(Font.Size > 0)
              {
                  pMemory += (Font.Size - 1);
              }
            #endif

            if(FontDescriptor.TotalSize == 0)
            {
                FontDescriptor.pAddress = nullptr;
            }
            else
            {
                SizeCount = 0;
                while(SizeCount < FontDescriptor.TotalSize)
                {
                    Value = FontRaw[Font][Counter++];

                    // First Nibble
                    if(SizeCount < FontDescriptor.TotalSize)
                    {
                        BitCount = (uint8_t)(((Value >> 4) & 0x07) + 1);
                        BitValue = Value >> 7;

                        while(BitCount--)
                        {
                            if(BitValue)  *pMemory = 0xFF;
                            else          *pMemory = 0x00;
                            SizeCount++;
                            #ifdef LCD_INVERT_SCREEN
                                pMemory--;
                            #else
                                pMemory++;
                            #endif
                         }
                    }

                    // Second Nibble
                    if(SizeCount < FontDescriptor.TotalSize)
                    {
                        BitCount = (uint8_t)((Value & 0x07) + 1);
                        BitValue = (uint8_t)((Value & 0x08) >> 3);

                        while(BitCount--)
                        {
                            if(BitValue)  *pMemory = 0xFF;
                            else          *pMemory = 0x00;
                            SizeCount++;
                            #ifdef LCD_INVERT_SCREEN
                                pMemory--;
                            #else
                                pMemory++;
                            #endif
                        }
                    }
                }
            }

            // Save the descriptor into database
            DB_Central.Set(&FontDescriptor, GFX_FONT_DESC_INFO, Font, Character);

            // Save Free SDRAM Pointer
            if(FontDescriptor.TotalSize != 0)
            {
                FontDescriptor.pAddress += FontDescriptor.TotalSize;
                DB_Central.Set(&FontDescriptor.pAddress, GFX_FREE_RAM_POINTER, 0, 0);
            }
        }
    }
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           PrintFont
//
//  Parameter(s):   FontDescriptor_t*   pDescriptor
//                  Cartesian_t*        pPos
//  Return:         none
//
//  Description:    This function will print a font to drawing layer with the drawing color
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void PrintFont(FontDescriptor_t* pDescriptor, Cartesian_t* pPos)
{
  #ifdef GRAFX_USE_SOFT_PRINT_FONT

  #else
    myGrafx->PrintFont(pDescriptor, pPos);
  #endif
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX
