//-------------------------------------------------------------------------------------------------
//
//  File : Lib_strnchr.cpp
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
#include "lib_string.h"
#include "grafx_cfg.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: STR_strnchr
//
//   Parameter(s):  const char*      pStr        Destination buffer
//                  size_t           Size        Size max to scan
//                  char             Chr         Character to found
//   Return Value:  const char*      pPtr        Pointer if found or nullptr
//
//   Description:   Locate first occurence of caracter in a string up to the EOL
//                  EOL is part of the string and can be use to find the end of string
//
//-------------------------------------------------------------------------------------------------
char* STR_strnchr(char* pStr, size_t Size, char Chr)
{
    size_t Count = 0;


    if(Size > DIGINI_MAX_PRINT_SIZE)
    {
        Size = DIGINI_MAX_PRINT_SIZE;
    }

    while((*pStr != '\0') && (*pStr != Chr) && (Count < Size))
    {
        pStr++;
        Count++;
    }

    if((*pStr == '\0') && (Chr != '\0'))
    {
        pStr = nullptr;
    }
    return pStr;
}

