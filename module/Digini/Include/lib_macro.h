//-------------------------------------------------------------------------------------------------
//
//  File : lib_macro.h
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

#pragma once

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define VAR_UNUSED(v)                   ((void)(v))

#define AbsMin(V,T)                     ((V < T) ? T : V)
#define AbsMax(V,T)                     ((V > T) ? T : V)
#define Toggle(A)                       (A = (uint8_t)(1 - A))
#define CheckOption(A,B)                (((B) & (A)) > 0)
#define sizearray(A)                    (sizeof(A) / sizeof(A[0]))
#define ABS(X)                          ((X) > 0 ? (X) : -(X))

#define swap8(A)                        ((A << 4) | (A >> 4))
#define swap16(A)                       ((A << 8) | (A >> 8))
#define swap32(A)                       ((A << 16) | (A >> 16))

// Macro to convert 2 ascii character that represent a Byte in text to a real HEX value
#define Asc2Hex(A,B)                    (((((A-=48)>9?(A-=7):A)>15)?(A-=32):A)<<4)+((((B-=48)>9?(B-=7):B)>15)?(B-=32):B)

// Macro to convert 1 ascii character that represent a Byte in text to a real HEX value
#define AscHex(A)                       ((((A-=48)>9?(A-=7):A)>15)?(A-=32):A)

// Bit operation on uint8_t
//#define SetBit(DATA, BIT)             (DATA |=  (0x01 << BIT))
//#define ClrBit(DATA, BIT)             (DATA &=  (0xFF ^ (0x01 << BIT)))
#define CheckBit(DATA, BIT)             (DATA &   (0x01 << BIT))
#define ToggleBit(DATA, BIT)            (DATA ^=  (0x01 << BIT))


// Bit operation on uint16_t
#define SetBit_u16(DATA, BIT)           (DATA |=  ((uint16_t)0x01 << BIT))
#define ClrBit_u16(DATA, BIT)           (DATA &=  (0xFFFF ^ ((uint16_t)0x01 << BIT)))
#define CheckBit_u16(DATA, BIT)         (DATA &   ((uint16_t)0x01 << BIT))
#define ToggleBit_u16(DATA, BIT)        (DATA ^=  ((uint16_t)0x01 << BIT))

// Bit operation on LONG data
#define SetBit_u32(DATA, BIT)           (DATA |=  ((uint32_t)0x01 << BIT))
#define ClrBit_u32(DATA, BIT)           (DATA &=  (0xFFFFFFFF ^ ((uint32_t)0x01 << BIT)))
#define CheckBit_u32(DATA, BIT)         (DATA &   ((uint32_t)0x01 << BIT))
#define ToggleBit_u32(DATA, BIT)        (DATA ^=  ((uint32_t)0x01 << BIT))

// Create uint32_t value fron 4 uint8_t value ou ascii char
#define U32MACRO(A,B,C,D)               ((uint32_t(D) << 24) + (uint32_t(C) << 16) + (uint32_t(B) << 8) + uint32_t(A))
// Create uint32_t value fron 4 uint8_t value ou ascii char
#define U16MACRO(A,B)                   ((uint16_t(B) << 8) + uint16_t(A))

#define GetTick()                        nOS_GetTickCount()
#define TickHasTimeOut(start_time,delay) ((nOS_GetTickCount() - (start_time)) > ((nOS_TickCounter)delay))

#define CVT_HOUR_TO_SECOND(H)           ((H) * 3600)


