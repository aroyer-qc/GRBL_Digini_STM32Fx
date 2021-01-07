//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_box.cpp
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

//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawBox
//
//   Parameter(s):  uint16_t    PosX
//                  uint16_t    PosY
//                  uint16_t    Length
//                  uint16_t    Height
//                  uint16_t    Thickness
//   Return Value:  none
//
//   Description:   Draw a box of the desired colour
//
//-------------------------------------------------------------------------------------------------
void DrawBox(uint16_t PosX, uint16_t PosY, uint16_t Length, uint16_t Height, uint16_t Thickness)
{
  #ifdef GRAFX_USE_SOFT_BOX
  #else
    myGrafx->DrawBox(PosX, PosY, Length, Height, Thickness);
  #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawBox
//
//   Parameter(s):  Box_t*      pBox
//                  uint16_t   Thickness
//   Return Value:  none
//
//   Description:   Draw a box of the desired colour
//
//-------------------------------------------------------------------------------------------------
void DrawBox(Box_t* pBox, uint16_t Thickness)
{
  #ifdef GRAFX_USE_SOFT_BOX
    uint16_t X2 = pBox->Pos.X + pBox->Size.X;
    uint16_t Y2 = pBox->Pos.Y + pBox->Size.Y;

    myGrafx->DrawVLine(pBox->Pos.u_16.X, pBox->Pos.Y, wY2, Thickness);
    myGrafx->DrawVLine(wX2 - Thickness,  pBox->Pos.Y, wY2, Thickness);
    myGrafx->DrawHLine(pBox->Pos.u_16.Y, pBox->Pos.X, wX2, Thickness);
    myGrafx->DrawHLine(wY2 - Thickness,  pBox->Pos.X, wX2, Thickness);
  #else
    myGrafx->DrawBox(pBox->Pos.X, pBox->Pos.Y, pBox->Size.Width, pBox->Size.Height, Thickness);
  #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawDebugBox
//
//   Parameter(s):  Box_t*      pBox
//                  uint16_t   Thickness
//   Return Value:  none
//
//   Description:   Draw a surrounding box around the widget from the debug config color
//
//-------------------------------------------------------------------------------------------------
void DrawDebugBox(Box_t* pBox)
{
    myGrafx->DrawBox(pBox->Pos.X - 1, pBox->Pos.Y - 1, pBox->Size.Width + 2, pBox->Size.Height + 2, 1);
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX


