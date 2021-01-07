//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_line.cpp
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
// Forward declaration(s)
//-------------------------------------------------------------------------------------------------

static float    ipart       (float x);
static int      round       (float x);
static float    fpart       (float x);
static float    rfpart      (float x);

//-------------------------------------------------------------------------------------------------
// Local function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function Name: Support function Xiaolin Wu's Line Algorithm
//
//-------------------------------------------------------------------------------------------------
static float ipart(float x)
{
    return static_cast<int>(x);
}

//-------------------------------------------------------------------------------------------------

static int round(float x)
{
	return ipart(x + 0.5f);
}

//-------------------------------------------------------------------------------------------------

static float fpart(float x)
{
	return x - ipart(x);
}

//-------------------------------------------------------------------------------------------------

static float rfpart(float x)
{
	return 1 - fpart(x);
}

//-------------------------------------------------------------------------------------------------

static void plot(uint16_t x, uint16_t y, uint16_t c)
{
//    uint32_t Color = CLayer::GetColor();

    CLayer::SetColor(RED);
    myGrafx->DrawPixel(x, x);
    //Pixel(x, y, c); // avec la luminosité c (avec 0 ≤ c ≤ 1)
}

//-------------------------------------------------------------------------------------------------
// Global function(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawHLine
//
//   Parameter(s):  uint16_t    Y
//                  uint16_t    X1
//                  uint16_t    X2
//                  uint16_t    Tickness
//   Return Value:  none
//
//   Description:   Draw a horizontal
//
//-------------------------------------------------------------------------------------------------
void DrawHLine(uint16_t Y, uint16_t X1, uint16_t X2, uint16_t Thickness)
{
    //Cartesian_t Size;

    //Size = GetMaxSize();

    // Limit length to maximum of the layer
    //if(X2 > Size.X)
    //{
    //    X2 = Size.X;
    //}

    //if(Y >= Size.Y)              return;                      // Nothing to do line outside range
    //if((Y + Thickness) > Size.Y) Thickness -= (Size.Y - Y);   // Remove unnecessary line

    #ifdef GRAFX_USE_SOFT_HLINE
        // TO DO write code if horizontal line is done in software
    #else
        myGrafx->DrawHLine(Y, X1, X2, Thickness);
    #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawVLine
//
//   Parameter(s):  uint16_t    X
//                  uint16_t    Y1
//                  uint16_t    Y2
//                  uint16_t    Tickness
//   Return Value:  none
//
//   Description:   Draw a vertical line of desire thickness
//
//-------------------------------------------------------------------------------------------------
void DrawVLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t Thickness)
{
    //Cartesian_t Size;

    //Size = GetMaxSize();

    // Limit length to maximum of the layer
    //if(Y2 > Size.Y)
    //{
    //    Y2 = Size.Y;
    //}

    //if(X >= Size.X)              return;                        // Nothing to do line outside range
    //if((X + Thickness) > Size.X) Thickness -= (Size.X - X);     // Remove unnecessary line

    #ifdef GRAFX_USE_SOFT_HLINE
        // TO DO write code if vertical line is done in software
    #else
        myGrafx->DrawVLine(X, Y1, Y2, Thickness);
    #endif
}


//-------------------------------------------------------------------------------------------------
//
//   Function Name: DrawDLine
//
//   Parameter(s):  float    X1
//                  float    Y1
//                  float    X2
//                  float    Y2
//   Return Value:  none
//
//   Description:   Draw a diagonal line of desire thickness using the Xiaolin Wu Algorithm
//                  This algorithm allow line with anti-aliasing ( improvement on Bresenham
//
//-------------------------------------------------------------------------------------------------
void DrawDLine(float X1, float Y1, float X2, float Y2)
{
    int16_t DeltaX;
    int16_t DeltaY;
    float   Gradient;
    float   xEnd;
    float   yEnd;
    float   xGap;
    int16_t xPxl1;
    int16_t yPxl1;
    int16_t xPxl2;
    int16_t yPxl2;
    float   Intery;

    DeltaX = X2 - X1;
    DeltaY = Y2 - Y1;

    if(abs(DeltaX) > abs(DeltaY))
    {
        // Handle horizontal line
        if(X2 < X1)
        {
            float TP;
            TP = X1; X1 = X2; X2 = TP;
            TP = Y1; Y1 = Y2; Y2 = TP;
        }

        Gradient = DeltaY / DeltaX;

        // Handle first end of line point
        xEnd = static_cast<float>(round(X1));
        yEnd = Y1 + Gradient * (xEnd - X1);
        xGap = rfpart(X1 + 0.5);

        xPxl1 = static_cast<int>(xEnd);
        yPxl1 = ipart(yEnd);

        // Add the first endpoint
        plot(xPxl1, yPxl1, rfpart(yEnd) * xGap);
        plot(xPxl1, yPxl1 + 1, fpart(yEnd) * xGap);

        Intery = yEnd + Gradient;

        // Handle second end of line point
        xEnd = static_cast<float>(round(X2));
        yEnd = Y2 + Gradient * (xEnd - X2);
        xGap = fpart(X2 + 0.5);

        xPxl2 = static_cast<int>(xEnd);
        yPxl2 = ipart(yEnd);

        plot(xPxl2, yPxl2, rfpart(yEnd) * xGap);
        plot(xPxl2, yPxl2 + 1, fpart(yEnd) * xGap);

        // Main loop
        for(int16_t x = xPxl1 + 1; x < xPxl2; x++)
        {
            plot(x, ipart(Intery), rfpart(Intery));
            plot(x, ipart(Intery) + 1, fpart(Intery));
            Intery += Gradient;
        }
    }
    else
    {
         // TODO Handle horizontal lines by swapping X and Y
    }
}

//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX

