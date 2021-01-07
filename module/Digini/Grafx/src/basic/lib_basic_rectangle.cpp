//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_rectangle.cpp
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
//   Function Name: DrawRectangle
//
//   Parameter(s):  uint16_t    X
//                  uint16_t    Y
//                  uint16_t    Width
//                  uint16_t    Height
//    Return Value: none
//
//   Description:   Draw a rectangle
//
//-------------------------------------------------------------------------------------------------
void DrawRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
    Box_t Box;

    Box.Pos.X       = X;
    Box.Pos.Y       = Y;
    Box.Size.Width  = Width;
    Box.Size.Height = Height;

    #ifdef GRAFX_USE_SOFT_FILL
        // TO DO write code if fill is done in software
    #else
        myGrafx->DrawRectangle(&Box);
    #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawRectangle
//
//   Parameter(s):  Box_t*    pBox
//    Return Value: none
//
//   Description:   Draw a rectangle
//
//-------------------------------------------------------------------------------------------------
void DrawRectangle(Box_t* pBox)
{
    #ifdef GRAFX_USE_SOFT_FILL
        // TO DO write code if fill is done in software
    #else
        myGrafx->DrawRectangle(pBox);
    #endif
}


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX

