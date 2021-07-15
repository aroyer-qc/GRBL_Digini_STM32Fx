//-------------------------------------------------------------------------------------------------
//
//  File : lib_utility.h
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
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_typedef.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define     LIB_MASK_CCITT                  0x1021
#define     LIB_MASK_CRC                    0xA005


#define     LIB_memchr                      memchr
#define     LIB_memcmp                      memcmp
#define     LIB_MemCopyOdd                  memcpy
#define     LIB_MemCopyEven                 memcpy
#define     LIB_memcpy                      memcpy
#define     LIB_MemSetBYTE                  memset
#define     LIB_MemSetWORD(d,v,c)           memset((d),(v),(c)<<1)
#define     LIB_MemSetDWORD(d,v,c)          memset((d),v,(c)<<2)
#define     LIB_MemClrBYTE(d,c)             memset((d),0,c)
#define     LIB_MemClrWORD(d,c)             memset((d),0,(c)<<1)
#define     LIB_MemClrDWORD(d,c)            memset((d),0,(c)<<2)
#define     LIB_BackMemCopyOdd              memmove
#define     LIB_BackMemCopyEven             memmove
#define     LIB_AlignPointer(Ptr)           {Ptr += ((4 - (uint32_t(Ptr) % 4) ) % 4);}

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

uint8_t     LIB_1AscHex                     (const char* pSrc);
uint8_t     LIB_1AscHex                     (const wchar_t* pSrc);
uint8_t     LIB_2AscHex                     (const char* pSrc);
uint8_t     LIB_2AscHex                     (const wchar_t* pSrc);
uint16_t    LIB_4AscHex                     (const wchar_t* pSrc);
uint16_t    LIB_4AscHex                     (const char* pSrc);
uint32_t    LIB_6AscHex                     (const char* pSrc);
uint32_t    LIB_6AscHex                     (const wchar_t* pSrc);

uint8_t     LIB_2BcdDec                     (uint8_t BCD_Value);
uint16_t    LIB_4BcdDec                     (uint16_t BCD_Value);

uint8_t     LIB_2DecBcd                     (uint8_t DecimalValue);
uint16_t    LIB_4DecBcd                     (uint16_t DecimalValue);

void        LIB_uint16_t_Swap               (uint16_t* pSwap);
void        LIB_uint32_t_Swap               (uint32_t* pSwap);

uint16_t    LIB_TriAngleToOppositeCoord     (uint16_t Angle, uint16_t AdjacentSize);        // For 0 - 45 degree
uint16_t    LIB_SinCos                      (uint16_t Angle, uint16_t Radius);              // For 0 - 45 degree

void        LIB_ProcessFloatToString        (char* pStr, double Value, int NumberOfDecimal);

void		LIB_Delay_uSec					(uint32_t Delay);
void		LIB_Delay_mSec					(uint32_t Delay);

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class RawArray
{
    public:

                    RawArray        ();
                    RawArray        (void* pBuffer);
                    RawArray        (void* pBuffer, size_t Size);
                    ~RawArray       ();

        void        append          (uint8_t data);
        void        append          (uint16_t data);
        void        append          (uint32_t data);
        uint8_t     at              (uint32_t Index);
        void        clear           ();
        uint8_t*    data            ();
        void        fill            (uint8_t data);
        void        insert          (uint32_t Index, uint8_t data);
        uint8_t     last            ();
        void        remove          (uint32_t Index);
        bool        replace         (uint32_t Index, uint8_t data);
        bool        replace         (uint32_t Index, uint16_t data);
        bool        replace         (uint32_t Index, uint32_t data);
        void        resize          (size_t Size);
        void        setBuffer       (uint8_t* pBuffer);
        size_t      size            ();
        uint8_t     takeLast        ();
        uint16_t    uint16_At       (uint32_t Index);
        uint32_t    uint32_At       (uint32_t Index);
        uint16_t    uint16_Last     ();
        uint32_t    uint32_Last     ();
        uint16_t    uint16_TakeLast ();
        uint32_t    uint32_TakeLast ();

    private:

        uint8_t*            m_pBuffer;
        size_t              m_Size;
};

//-------------------------------------------------------------------------------------------------



/*

uint8_t                LIB_Checksum                        (const void* pMem, uint16_t wCount);
uint32_t               LIB_SwapDWORD                       (uint32_t dwData);
uint16_t               LIB_SwapWORD                        (uint16_t wData);
uint32_t               LIB_Bits_Array                      (uint32_t* pArray, uint8_t byStart, uint8_t bySize);
void                   LIB_Copy_BYTETable_To_DWORDTable    (uint32_t* pdwDest, uint8_t *pbySource, uint8_t bySize);
void                   LIB_Copy_DWORDTable_To_BYTETable    (uint8_t* pbyDest, uint32_t* pdwSource, uint8_t bySize);
void                   LIB_CRC16                           (uint16_t* pCrc, uint8_t byData);
void                   LIB_StringByteToWord                (void* pOut, void *pIn);
uint8_t                LIB_DecHex                          (uint8_t bySource);
uint16_t               LIB_3AscHex                         (void* pSrc);
uint16_t               LIB_4AscHex                         (void* pSrc);
uint32_t               LIB_5AscHex                         (void* pSrc);
uint32_t               LIB_6AscHex                         (void* pSrc);
uint32_t               LIB_8AscHex                         (void* pSrc);
uint8_t                LIB_1AscDec                         (void* pSrc);
uint8_t                LIB_2AscDec                         (void* pSrc);
uint16_t               LIB_3AscDec                         (void* pSrc);
uint16_t               LIB_4AscDec                         (void* pSrc);
uint32_t               LIB_5AscDec                         (void* pSrc);
uint32_t               LIB_6AscDec                         (void* pSrc);
uint32_t               LIB_7AscDec                         (void* pSrc);
uint32_t               LIB_8AscDec                         (void* pSrc);
uint32_t               LIB_10AscDec                        (void* pSrc);
uint8_t                LIB_1AscDecp                        (void* pSrc);
uint8_t                LIB_2AscDecp                        (void* pSrc);
uint16_t               LIB_3AscDecp                        (void* pSrc);
uint16_t               LIB_4AscDecp                        (void* pSrc);
uint32_t               LIB_5AscDecp                        (void* pSrc);
uint32_t               LIB_6AscDecp                        (void* pSrc);
uint32_t               LIB_8AscDecp                        (void* pSrc);
uint16_t               LIB_ExtractMedian                   (uint16_t *pValue, uint8_t byNbSamples);
uint8_t                LIB_StringToGUID                    (void *pSrc, uint8_t *pGUID);

*/
//-------------------------------------------------------------------------------------------------

