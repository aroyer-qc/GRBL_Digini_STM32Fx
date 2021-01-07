//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_pie.cpp
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
// Private Function(s)
//-------------------------------------------------------------------------------------------------

static void DrawSector(Skin_e Image, Circle_t* pCircle, uint16_t StartAngle, uint16_t EndAngle, uint16_t Number, BlendMode_e BlendMode);

//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawPie
//
//   Parameter(s):  Skin_e               Image
//                  Pie_t*               pPie
//                  BlendMode_e          BlendMode
//   Return Value:  none
//
//   Description:   Copy a part of a circle from linear memory region to pie shape in destination
//
//-------------------------------------------------------------------------------------------------
void DrawPie(Skin_e Image, Pie_t* pPie, BlendMode_e BlendMode)
{
    uint16_t   SectorStartAngle;
    uint16_t   SectorEndAngle;
    uint8_t    SectorNumber;

    if(pPie->EndAngle < pPie->StartAngle) pPie->EndAngle += 360;                                            // So we can work in positive direction

    for(uint16_t Angle = pPie->StartAngle; Angle < pPie->EndAngle; )                                        // Copy each sector that pie is crossing in part or in full
    {
        // This sector number
        SectorNumber = Angle / 90;

        // This sector angle start
        SectorStartAngle = Angle % 90;

        // This sector angle end
        SectorEndAngle = 90;
        if(pPie->EndAngle < (SectorEndAngle + (SectorNumber * 90)))
        {
           SectorEndAngle = pPie->EndAngle % 90;                                                            // Check if the pie end angle is in this sector
        }

        _DrawSector(Image, &pPie->Circle, SectorStartAngle, SectorEndAngle, SectorNumber % 4, BlendMode);  // Draw one the the 4 sector in full or partially

        // Increment angle for next sector
        Angle += (SectorEndAngle - SectorStartAngle);
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawPie
//
//   Parameter(s):  Skin_e               Image
//                  Cartesian_t          Position
//                  uint16_t            Radius
//                  uint16_t            StartAngle
//                  uint16_t            EndAngle
//                  BlendMode_e          BlendMode
//   Return Value:  none
//
//   Description:   Copy a part of a circle from linear memory region to pie shape in destination
//
//-------------------------------------------------------------------------------------------------
void DrawPie(Skin_e Image, Cartesian_t Position, uint16_t Radius, uint16_t StartAngle, uint16_t EndAngle, BlendMode_e BlendMode)
{
    Pie_t Pie;

    Pie.Circle.Pos         = Position;
    Pie.Circle.R           = Radius;
    Pie.StartAngle         = StartAngle;
    Pie.EndAngle           = EndAngle;

    DrawPie(Image, &Pie, BlendMode);
}

//-------------------------------------------------------------------------------------------------
//
//   Function Name: _DrawSector
//
//   Parameter(s):  Skin_e          Image
//                  Circle_t*            pCircle
//                  uint16_t            StartAngle
//                  uint16_t            EndAngle
//                  uint16_t            SectorNumber
//                  BlendMode_e          BlendMode
//   Return Value:  none
//
//   Description:   Draw one sector of the pie (4 sectors in full circle)
//
//-------------------------------------------------------------------------------------------------
static void _DrawSector(Skin_e Image, Circle_t* pCircle, uint16_t StartAngle, uint16_t EndAngle, uint16_t SectorNumber, BlendMode_e BlendMode)
{
    ImageInfo_t ImageInfo;
    Cartesian_t Origin;
    Cartesian_t StartPoint;
    Cartesian_t CornerPoint;
    Cartesian_t EndPoint;
    Cartesian_t SrcImageOffset;

    Origin.X = pCircle->Pos.X + pCircle->R;                // Fix origin at center of the circle
    Origin.Y = pCircle->Pos.Y + pCircle->R;

    // Fix the corner point (This might change if start and end are both smaller or greater)
    if((SectorNumber == 0) || (SectorNumber == 1)) CornerPoint.X = Origin.X + pCircle->R;
    else                                           CornerPoint.X = Origin.X - pCircle->R;

    if((SectorNumber == 1) || (SectorNumber == 2)) CornerPoint.Y = Origin.Y + pCircle->R;
    else                                           CornerPoint.Y = Origin.Y - pCircle->R;

    if((StartAngle == 0) && (EndAngle == 90))               // We copy the entire sector (StartPoint and EndPoint position does not matter)
    {
        StartPoint.X = CornerPoint.X;
        StartPoint.Y = Origin.Y;
        EndPoint.X   = Origin.X;
        EndPoint.Y   = CornerPoint.Y;
    }
    else if((StartAngle < 45) && (EndAngle > 45))           // Corner point is not going to change
    {
        EndAngle = 90 - EndAngle;

        if((SectorNumber == 0) || (SectorNumber == 2))
        {
            StartPoint.Y = CornerPoint.Y;
            EndPoint.X   = CornerPoint.X;

            if(SectorNumber == 0)
            {
                StartPoint.X = Origin.X + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                EndPoint.Y   = Origin.Y - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
            }
            else
            {
                StartPoint.X = Origin.X - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                EndPoint.Y   = Origin.Y + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
            }
        }
        else
        {
            StartPoint.X = CornerPoint.X;
            EndPoint.Y   = CornerPoint.Y;

            if(SectorNumber == 1)
            {
                StartPoint.Y = Origin.Y + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                EndPoint.X   = Origin.X + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
            }
            else
            {
                StartPoint.Y = Origin.Y - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                EndPoint.X   = Origin.X - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
            }
        }
    }
    else                                                    // Corner point will be recalculated
    {
        if(StartAngle < 45)                                 // Are the Start and End both below cornerpoint ?
        {
            if((SectorNumber == 0) || (SectorNumber == 2))
            {
                if(SectorNumber == 0)
                {
                    StartPoint.X = Origin.X + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.X   = Origin.X + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }
                else
                {
                    StartPoint.X = Origin.X - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.X   = Origin.X - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }

                CornerPoint.X = StartPoint.X;
                StartPoint.Y  = CornerPoint.Y;
                EndPoint.Y    = CornerPoint.Y;
            }
            else //((SectorNumber == 1) || (SectorNumber == 3))
            {
                if(SectorNumber == 1)
                {
                    StartPoint.Y = Origin.Y + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.Y   = Origin.Y + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }
                else
                {
                    StartPoint.Y = Origin.Y - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.Y   = Origin.Y - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }

                CornerPoint.Y = StartPoint.Y;
                StartPoint.X  = CornerPoint.X;
                EndPoint.X    = CornerPoint.X;
            }
        }
        else
        {
            EndAngle   = 90 - EndAngle;
            StartAngle = 90 - StartAngle;

            if((SectorNumber == 0) || (SectorNumber == 2))
            {
                if(SectorNumber == 0)
                {
                    StartPoint.Y = Origin.Y - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.Y   = Origin.Y - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }
                else
                {
                    StartPoint.Y = Origin.Y + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.Y   = Origin.Y + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }

                CornerPoint.Y = StartPoint.Y;
                StartPoint.X  = CornerPoint.X;
                EndPoint.X    = CornerPoint.X;
            }
            else //((SectorNumber == 1) || (SectorNumber == 3))
            {
                if(SectorNumber == 1)
                {
                    StartPoint.X = Origin.X + LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.X   = Origin.X + LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }
                else
                {
                    StartPoint.X = Origin.X - LIB_TriAngleToOppositeCoord(StartAngle, pCircle->R);
                    EndPoint.X   = Origin.X - LIB_TriAngleToOppositeCoord(EndAngle, pCircle->R);
                }

                CornerPoint.X = StartPoint.X;
                StartPoint.Y  = CornerPoint.Y;
                EndPoint.Y    = CornerPoint.Y;
            }
        }
    }

    SrcImageOffset.X = Origin.X;
    if(StartPoint.X < SrcImageOffset.X)  SrcImageOffset.X = StartPoint.X;
    if(CornerPoint.X < SrcImageOffset.X) SrcImageOffset.X = CornerPoint.X;
    if(EndPoint.X < SrcImageOffset.X)    SrcImageOffset.X = EndPoint.X;
    SrcImageOffset.X -= pCircle->Pos.X;

    SrcImageOffset.Y = Origin.Y;
    if(StartPoint.Y < SrcImageOffset.Y)  SrcImageOffset.Y = StartPoint.Y;
    if(CornerPoint.Y < SrcImageOffset.Y) SrcImageOffset.Y = CornerPoint.Y;
    if(EndPoint.Y < SrcImageOffset.Y)    SrcImageOffset.Y = EndPoint.Y;
    SrcImageOffset.Y -= pCircle->Pos.Y;

    DB_Central.Get(&ImageInfo, GFX_IMAGE_INFO, Image, 0);
    PatternPolygon(ImageInfo.pPointer, ImageInfo.Size.Width, SrcImageOffset, Origin, StartPoint, CornerPoint, EndPoint, ImageInfo.PixelFormat, BlendMode);
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_GRAFX

