//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_copy_linear.cpp
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
#include "bsp.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: CopyLinear
//
//   Parameter(s):  Skin_e               Image
//                  Cartesian_t          Position
//                  BlendMode_e          BlendMode
//   Return Value:  none
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//-------------------------------------------------------------------------------------------------
void CopyLinear(Skin_e Image, Cartesian_t Position, BlendMode_e BlendMode)
{
    ImageInfo_t ImageInfo;
    Box_t       Box;

    if(Image != INVALID_IMAGE)
    {
        DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, uint16_t(Image), 0);
        Box.Pos.X       = Position.X;
        Box.Size.Width  = ImageInfo.Size.Width;
        Box.Pos.Y       = Position.Y;
        Box.Size.Height = ImageInfo.Size.Height;
        myGrafx->CopyLinear(ImageInfo.pPointer, &Box, ImageInfo.PixelFormat, BlendMode);
    }
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: CopyLinear
//
//   Parameter(s):  void*               pSrc
//                  Box_t                Box
//                  PixelFormat_e        SrcPixelFormat
//                  BlendMode_e          BlendMode
//   Return Value:  none
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//-------------------------------------------------------------------------------------------------
void CopyLinear(void* pSrc, Box_t* pBox, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    myGrafx->CopyLinear(pSrc, pBox, SrcPixelFormat, BlendMode);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: CopyLinear
//
//   Parameter(s):  void*               pSrc
//                  uint16_t            X
//                  uint16_t            Y
//                  uint16_t            Width
//                  uint16_t            Height
//                  PixelFormat_e       SrcPixelFormat
//                  BlendMode_e         BlendMode
//   Return Value:  none
//
//   Description:   Copy a rectangle region from linear memory region to square memory area
//
//-------------------------------------------------------------------------------------------------
void CopyLinear(void* pSrc, uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode)
{
    Box_t Box;

    Box.Pos.X       = X;
    Box.Size.Width  = Width;
    Box.Pos.Y       = Y;
    Box.Size.Height = Height;
    myGrafx->CopyLinear(pSrc, &Box, SrcPixelFormat, BlendMode);
}


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX


