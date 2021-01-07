//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_polygon.cpp
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
#include "lib_define.h"
#include "lib_memory.h"

//-------------------------------------------------------------------------------------------------
// Private variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

    static uint16_t    MinY;
    static uint16_t    MaxY;
//    uint16_t*   GRAFX_pMin;
//    uint16_t*   GRAFX_pMax;
    static uint16_t   pMin[256];
    static uint16_t   pMax[256];

//-------------------------------------------------------------------------------------------------
// Private function(s)
//-------------------------------------------------------------------------------------------------

static void _DrawLineForPolygonFill(Cartesian_t Pos1, Cartesian_t Pos2, bool IsItDraw);
static void _DrawPolygon(void* pSrc, uint16_t SrcLineWidth, Cartesian_t SrcImageOffset, Cartesian_t Pos1, Cartesian_t Pos2, Cartesian_t Pos3, Cartesian_t Pos4, PolygonMode_e PolygonMode, PixelFormat_e SrcPixelFormat, BlendMode_e BlendMode);

//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawPolygon
//
//   Parameter(s):  Polygon_t*       pPolygon
//                  PolygonMode_e    PolygonMode
//   Return Value:  none
//
//   Description:   Draw a polygon on screen then fill it. this polygone has four points.
//
//   notes:         Option are:         POLY_FILL  -> for a completely fill circle
//                                      POLY_SHAPE -> for the contour shape only
//
//-------------------------------------------------------------------------------------------------
void DrawPolygon(Polygon_t* pPolygon, PolygonMode_e PolygonMode)
{
    Cartesian_t Dummy = {0};
    _DrawPolygon(nullptr, 0, Dummy, pPolygon->Pos1, pPolygon->Pos2, pPolygon->Pos3, pPolygon->Pos4, PolygonMode, DUMMY_PIXEL_FORMAT, NO_BLEND);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawPolygon
//
//   Parameter(s):  Cartesian_t      Pos1
//                  Cartesian_t      Pos2
//                  Cartesian_t      Pos3
//                  Cartesian_t      Pos4
//                  PolygonMode_e    PolygonMode
//   Return Value:  none
//
//   Description:   Draw a polygon on screen then fill it. this polygone has four points.
//
//   notes:         Option are:         POLY_FILL  -> for a completely fill circle
//                                      POLY_SHAPE -> for the contour shape only
//
//-------------------------------------------------------------------------------------------------
void DrawPolygon(Cartesian_t Pos1, Cartesian_t Pos2, Cartesian_t Pos3, Cartesian_t Pos4, PolygonMode_e PolygonMode)
{
    Cartesian_t Dummy = {0};
    _DrawPolygon(nullptr, 0, Dummy, Pos1, Pos2, Pos3, Pos4, PolygonMode, DUMMY_PIXEL_FORMAT, NO_BLEND);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawPatternPolygon
//
//   Parameter(s):  void*               pSrc
//                  uint16_t            SrcLineWidth
//                  Cartesian_t         SrcOffset
//                  Cartesian_t         Pos1
//                  Cartesian_t         Pos2
//                  Cartesian_t         Pos3
//                  Cartesian_t         Pos4
//                  PixelFormat_e       PixelFormat
//                  BlendMode_e         BlendMode
//   Return Value:  none
//
//   Description:   Draw a polygon on screen then fill it with a pattern source.
//                  this polygone has four points.
//
//-------------------------------------------------------------------------------------------------
void DrawPatternPolygon(void*         pSrc,
                        uint16_t      SrcLineWidth,
                        Cartesian_t   SrcOffset,
                        Cartesian_t   Pos1,
                        Cartesian_t   Pos2,
                        Cartesian_t   Pos3,
                        Cartesian_t   Pos4,
                        PixelFormat_e PixelFormat,
                        BlendMode_e   BlendMode)
{
    _DrawPolygon(pSrc, SrcLineWidth, SrcOffset, Pos1, Pos2, Pos3, Pos4, POLY_FILL, PixelFormat, BlendMode);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: _DrawPolygon
//
//   Parameter(s):  void*               pSrc
//                  uint16_t            SrcLineWidth,
//                  Cartesian_t         SrcImageOffset
//                  Cartesian_t         Pos1
//                  Cartesian_t         Pos2
//                  Cartesian_t         Pos3
//                  Cartesian_t         Pos4
//                  PolygonMode_e       PolygonMode
//                  PixelFormat_e       PixelFormat
//                  BlendMode_e         BlendMode
//
//   Return Value:  none
//
//   Description:   Draw a polygon on screen then fill it. this polygon has four points.
//
//   notes:         Option are:         POLY_FILL  -> for a completely fill circle
//                                      POLY_SHAPE -> for the contour shape only
//
//-------------------------------------------------------------------------------------------------
static void _DrawPolygon(void*         pSrc,
                         uint16_t      SrcLineWidth,
                         Cartesian_t   SrcImageOffset,
                         Cartesian_t   Pos1,
                         Cartesian_t   Pos2,
                         Cartesian_t   Pos3,
                         Cartesian_t   Pos4,
                         PolygonMode_e PolygonMode,
                         PixelFormat_e PixelFormat,
                         BlendMode_e   BlendMode)
{
    uint16_t    Size;
    uint32_t    i;
    bool        IsItDraw = false;

    MinY = 0xFFFF;
    MaxY = 0x0000;

    if(Pos1.Y < MinY) MinY = Pos1.Y;
    if(Pos2.Y < MinY) MinY = Pos2.Y;
    if(Pos3.Y < MinY) MinY = Pos3.Y;
    if(Pos4.Y < MinY) MinY = Pos4.Y;

    if(Pos1.Y > MaxY) MaxY = Pos1.Y;
    if(Pos2.Y > MaxY) MaxY = Pos2.Y;
    if(Pos3.Y > MaxY) MaxY = Pos3.Y;
    if(Pos4.Y > MaxY) MaxY = Pos4.Y;

    Size = (MaxY - MinY);

    // Fill a table with X start and X end for each line
    //GRAFX_pMin = (uint16_t*)pMemory->AllocAndSet  (Size * sizeof(uint16_t) * 2, 0xFF);
    //GRAFX_pMax = (uint16_t*)pMemory->AllocAndClear(Size * sizeof(uint16_t) * 2);

    for(i = 0; i < 256; i++)
    {
        pMin[i] = 0xFFFF;
        pMax[i] = 0x0000;
    }

    if(pSrc == nullptr) IsItDraw = true;
    _DrawLineForPolygonFill(Pos1, Pos2, IsItDraw);
    _DrawLineForPolygonFill(Pos2, Pos3, IsItDraw);
    _DrawLineForPolygonFill(Pos3, Pos4, IsItDraw);
    _DrawLineForPolygonFill(Pos4, Pos1, IsItDraw);

    if(pSrc == nullptr)
    {
        if(PolygonMode == POLY_FILL)
        {
            for(i = 0; i < Size; i++)
            {
                DrawHLine(i + MinY, pMin[i], pMax[i], 1);
            }
        }
    }
    else
    {
        uint32_t Address;
        uint16_t OffsetX = 0xFFFF;

        // Get the Pixel X position for SrcImageOffset.u_16.X
        for(i = 0; i < Size; i++)
        {
            if(pMin[i] < OffsetX) OffsetX = pMin[i];
        }

        for(i = 0; i < Size; i++)
        {
            Address  = (i + uint32_t(SrcImageOffset.Y)) * uint32_t(SrcLineWidth);                       // Offset for Y
            Address += uint32_t(SrcImageOffset.X);                                                      // + Base X offset in image
            Address += (pMin[i] - OffsetX);                                                         // + Polygon X Offset
  //Address *= uint32_t(CLayer.GetPixelSize(PixelFormat);                                            // * Pixel Size
            Address += uint32_t(pSrc);                                                                  // + Base address of image
            CopyLinear((void*)Address, pMin[i], MinY + i, pMax[i] - pMin[i], 1, PixelFormat, BlendMode);
        }
    }


    //pMemory->Free(GRAFX_pMax);
    //pMemory->Free(GRAFX_pMin);
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: _DrawLineForPolygonFill
//
//   Parameter(s):  Cartesian_t Pos1
//                  Cartesian_t Pos2
//                  bool        IsItDraw
//   Return Value:  none
//
//   Description:   Draw a diagonal line on screen
//
//   notes:         Do not use this to draw normal diagonal line as it is special for
//                  GRAFX_DrawPolygonFill
//
//-------------------------------------------------------------------------------------------------
static void _DrawLineForPolygonFill(Cartesian_t Pos1, Cartesian_t Pos2, bool IsItDraw)
{
    int16_t  DX2;
    int16_t  DY2;
    int16_t  DX;
    int16_t  DY;
    int16_t  IX;
    int16_t  IY;
    int16_t  Error;
    uint16_t Offset;
    uint16_t i;

    if(Pos1.X == Pos2.X)                                                                        // Process ertical line
    {
        for(i = Pos1.Y; i != Pos2.Y; ((Pos1.Y < Pos2.Y) ? i++ : i--))
        {
            Offset = i - MinY;
            if(IsItDraw == true) DrawPixel(Pos1.X, i);

            if(Pos1.X < pMin[Offset]) pMin[Offset] = Pos1.X;
            if(Pos1.X > pMax[Offset]) pMax[Offset] = Pos1.X;
        }
    }
    else if(Pos1.Y == Pos2.Y)                                                                   // Process Horizontal line
    {
        if(IsItDraw == true)
        {
            DrawHLine(Pos1.Y,
                     (Pos1.X > Pos2.X) ? Pos2.X : Pos1.X,
                     (Pos1.X > Pos2.X) ? Pos1.X : Pos2.X,
                     1);
        }
        Offset = Pos1.Y - MinY;
        if(Pos1.X < pMin[Offset])  pMin[Offset] = Pos1.X;
        if(Pos1.X > pMax[Offset])  pMax[Offset] = Pos1.X;
        if(Pos2.X < pMin[Offset])  pMin[Offset] = Pos2.X;
        if(Pos2.X > pMax[Offset])  pMax[Offset] = Pos2.X;
    }
    else
    {
        DX = (int16_t)Pos2.X - (int16_t)Pos1.X;
        DY = (int16_t)Pos2.Y - (int16_t)Pos1.Y;

        if(DX >= 0)
        {
            IX = 1;
        }
        else
        {
            IX = -1;
            DX = -DX;
        }

        if(DY >= 0)
        {
            IY = 1;
        }
        else
        {
            IY = -1;
            DY = -DY;
        }

        DX2 = (int16_t)DX << 1;
        DY2 = (int16_t)DY << 1;

        if(DX > DY)
        {
            Error = DY2 - DX;

            for(i = 0; i <= DX; i++)
            {
                Offset = Pos1.Y - MinY;
                if(Pos1.X < pMin[Offset])  pMin[Offset] = Pos1.X;
                if(Pos1.X > pMax[Offset])  pMax[Offset] = Pos1.X;

                if(IsItDraw == true) DrawPixel(Pos1.X, Pos1.Y);

                if(Error >= 0)
                {
                    Error  -= DX2;
                    Pos1.Y += IY;
                }
                Error  += DY2;
                Pos1.X += IX;
            }
        }
        else
        {
            Error = DX2 - DY;

            for(i = 0; i <= DY; i++)
            {
                Offset = Pos1.Y - MinY;
                if(Pos1.X < pMin[Offset])  pMin[Offset] = Pos1.X;
                if(Pos1.X > pMax[Offset])  pMax[Offset] = Pos1.X;

                if(IsItDraw == true) DrawPixel(Pos1.X, Pos1.Y);

                if(Error >= 0)
                {
                    Error  -= DY2;
                    Pos1.X += IX;
                }
                Error  += DX2;
                Pos1.Y += IY;
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX
