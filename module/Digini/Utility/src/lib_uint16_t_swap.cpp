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
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
//
//   Function Name: LIB_uint16_t_Swap
//
//   Parameter(s):  uint16_t*       pSwap
//   Return Value:  None
//
//   Description:   Swap all bytes in a 16 bits value
//
//-------------------------------------------------------------------------------------------------
void LIB_uint16_t_Swap(uint16_t* pSwap)
{
    s16_t Value;

    Value.u_8.u1 = ((s16_t*)pSwap)->u_8.u0;
    Value.u_8.u0 = ((s16_t*)pSwap)->u_8.u1;
    *pSwap = Value.u_16;
}

