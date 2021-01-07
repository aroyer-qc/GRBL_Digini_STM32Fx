//-------------------------------------------------------------------------------------------------
//
//  File : lib_uint16_t_swap.c
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
#include <stdio.h>
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: LIB_ProcessFloatToString
//
//   Parameter(s):  char*       pStr
//                  int         NumberOfDecimal       Number of decimal to keep.
//   Return Value:  None
//
//   Description:   will remove all but the first N decimals,
//                  then strip off the trailing zeros.
//                  Also decimal point if they were all zeros.
//
//-------------------------------------------------------------------------------------------------
void LIB_ProcessFloatToString(char* pStr, double Value, int NumberOfDecimal)
{
    char* pStrPtr;
    int   Count;
//void nDecimals (char *s, double d, int n) {
    int Size;
    double d2;

    // Allow for negative.

    d2   = (Value >= 0) ? Value : -Value;
    Size = (Value >= 0) ? 0 : 1;

    // Add one for each whole digit (0.xx special case).

    if(d2 < 1)
    {
        Size++;
    }

    while(d2 >= 1)
    {
        d2 /= 10.0;
        Size++;
    }

    // Adjust for decimal point and fractionals.
    Size += 1 + NumberOfDecimal;

    // Create format string then use it.
    sprintf(pStr, "%*.*f", Size, NumberOfDecimal, Value);

    //----------------------------
    // Stripping trailling zero's

    pStrPtr = strchr(pStr, '.');            // Find decimal point, if any.

    if(pStrPtr != nullptr)
    {
        Count = NumberOfDecimal;            // Adjust for more or less decimals.

        while(Count >= 0)                   // Maximum decimals allowed.
        {
             Count--;
             if (*pStrPtr == '\0')          // If there's less than desired.
             {
                 break;
             }

             pStrPtr++;                     // Next character.
        }

        *pStrPtr-- = '\0';                  // Truncate string.

        while(*pStrPtr == '0')              // Remove trailing zeros.
        {
            *pStrPtr-- = '\0';
        }

        if(*pStrPtr == '.')                 // If all decimals were zeros, remove ".".
        {
            *pStrPtr = '\0';
        }
    }
}
