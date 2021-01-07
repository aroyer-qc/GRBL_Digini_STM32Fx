//-------------------------------------------------------------------------------------------------
//
//  File : lib_4aschex.cpp
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
//   Function Name: LIB_4AscHex
//
//   Parameter(s):  const char* or const wchar_t*      Pointer on the first caracter
//   Return:        uint16_t                           16 Bit's value
//
//   Description:   Extract from 4 Ascii caracter to 16 bit's HEX value (uint16_t)
//
//   Note:          Overloaded function
//
//-------------------------------------------------------------------------------------------------
uint16_t LIB_4AscHex(const char* pSrc)
{
    uint16_t    Data;
    char*       _pSrc;

    _pSrc  = (char*)pSrc;
    Data   = (LIB_2AscHex(_pSrc) << 8);
    _pSrc += 2;
    Data  |=  LIB_2AscHex(_pSrc);
    return Data;
}

//-------------------------------------------------------------------------------------------------

uint16_t LIB_4AscHex(const wchar_t* pSrc)
{
    uint16_t    Data;
    wchar_t*    _pSrc;

    _pSrc  = (wchar_t*)pSrc;
    Data   = (LIB_2AscHex(_pSrc) << 8);
    _pSrc += 2;
    Data  |=  LIB_2AscHex(_pSrc);

    return Data;
}

//-------------------------------------------------------------------------------------------------

