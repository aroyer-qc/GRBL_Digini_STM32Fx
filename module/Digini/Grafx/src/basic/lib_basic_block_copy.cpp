#if 0
//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_block_copy.cpp
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
//   Function Name: BlockCopy
//
//   Parameter(s):  void*            pSrc
//                  Box_t            Box
//                  Cartesian_t*     pDstPos
//                  PixelFormat_e    SrcPixelFormat
//                  BlendMode        BlendMode
//   Return Value:  none
//
//   Description:   Copy a rectangle region from square memory region to another square memory
//                  region
//
//-------------------------------------------------------------------------------------------------
void BlockCopy(void* pSrc, Box_t* pBox, Cartesian_t* pDstPos, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    myGrafx->BlockCopy(pSrc, pBox, pDstPos, SrcPixelFormat, BlendMode);
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: BlockCopy
//
//   Parameter(s):  void*               pSrc
//                  uint16_t            X
//                  uint16_t            Y
//                  uint16_t            Width
//                  uint16_t            Height
//                  uint16_t            DstX
//                  uint16_t            DstY
//                  PixelFormat_e       SrcPixelFormat
//                  BlendMode_e         BlendMode
//   Return Value:  none
//
//   Description:   Copy a rectangle region from square memory region to another square memory
//                  region
//
//-------------------------------------------------------------------------------------------------
void BlockCopy(void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t DstX, uint16_t DstY, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    Box_t        Box;
    Cartesian_t  Pos;

    Box.Pos.X       = X;
    Box.Size.Width  = Width;
    Box.Pos.Y       = Y;
    Box.Size.Height = Height;
    Pos.X           = DstX;
    Pos.Y           = DstY;

    myGrafx->BlockCopy(pSrc, &Box, &Pos, SrcPixelFormat, BlendMode);
}

//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX


#endif
