//-------------------------------------------------------------------------------------------------
//
//  File : lib_basic_alpha.cpp
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

//static void    PrecomputeAlphaTable       ();            // TODO change for externally accessible
// ...



#ifdef GRAFX_USE_SOFT_ALPHA
// TO DO add other mode if required
// this is only for RGB565
static void GRAFX_PrecomputeAlphaTable()
{
    uint32_t   c;
    uint32_t   a;
    uint16_t    d;

    for(c = 0; c < 32; c++)
    {
        for(a = 0; a < 32; a++)
        {
            d = ((c * a) / 31) & 31;
            d <<= 11;
            g_AlphaTableRed[c][a] = d;
        }
    }
    for(c = 0; c < 64; c++)
    {
        for(a = 0; a < 64; a++)
        {
            d = ((c * a) / 63) & 63;
            d <<= 5;
            g_AlphaTableGreen[c][a] = d;
        }
    }
    for(c = 0; c < 32; c++)
    {
        for(a = 0; a < 32; a++)
        {
            d = ((c * a) / 31) & 31;
            g_AlphaTableBlue[c][a] = d;
        }
    }
}

#endif


//-------------------------------------------------------------------------------------------------
#endif // DIGINI_USE_GRAFX


