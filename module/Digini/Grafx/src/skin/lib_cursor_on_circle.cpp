//-------------------------------------------------------------------------------------------------
//
//  File : lib_cursor_on_circle.cpp
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
//   Function Name: GRAFX_CursorOnCircle
//
//   Parameter(s):  Skin_e               Image
//                  Cartesian_t*         pPos
//                  uint16_t            Radius
//                  uint16_t            Angle
//
//   Return Value:  none
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
void GRAFX_CursorOnCircle(Skin_e Image, Cartesian_t* pPos, uint16_t Radius, uint16_t Angle)
{
    uint16_t    PosX;
    uint16_t    PosY;
    uint16_t    OffX;
    uint16_t    OffY;
    uint8_t     Sector;
    uint16_t    CosAngle;
    uint16_t    SinAngle;
    ImageInfo_t  ImageInfo;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, Image, 0);

    Sector = Angle / 90;

    if((Sector == 1) || (Sector == 3))
    {
        CosAngle = 90 - (Angle % 90);
        SinAngle = Angle % 90;
    }
    else
    {
        CosAngle = Angle % 90;
        SinAngle = 90 - (Angle % 90);
    }

    OffX = LIB_SinCos(CosAngle, Radius - ImageInfo.Size.Width /2);
    OffY = LIB_SinCos(SinAngle, Radius - ImageInfo.Size.Height /2);

    PosX = pPos->X + (Radius - (ImageInfo.Size.Width / 2));
    PosY = pPos->Y + (Radius - (ImageInfo.Size.Height / 2));

    switch(Sector)
    {
        case 0: PosX += OffX; PosY -= OffY; break;
        case 1: PosX += OffX; PosY += OffY; break;
        case 2: PosX -= OffX; PosY += OffY; break;
        case 3: PosX -= OffX; PosY -= OffY; break;
    }

    CopyLinear(ImageInfo.pPointer, PosX, PosY, ImageInfo.Size.Width, ImageInfo.Size.Width, ImageInfo.PixelFormat, ALPHA_BLEND);
}


//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

