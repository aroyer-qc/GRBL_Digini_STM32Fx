//-------------------------------------------------------------------------------------------------
//
//  File : lib_4decbcd.cpp
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
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: LIB_4DecBcd
//
//   Parameter(s):  uint16_t     DecimalValue        Decimal value
//   Return:        uint16_t                         BCD coded value
//
//   Description:   Extract uint16_t BCD coded value fron a 0-9999 value
//
//-------------------------------------------------------------------------------------------------
uint16_t LIB_4DecBcd(uint16_t DecimalValue)
{
    uint16_t BCD_Value;

    BCD_Value  = uint16_t(LIB_2DecBcd(uint8_t(DecimalValue / 100))) << 8;
    BCD_Value |= uint16_t(LIB_2DecBcd(uint8_t(DecimalValue % 100)));
    return BCD_Value;
}

//-------------------------------------------------------------------------------------------------
