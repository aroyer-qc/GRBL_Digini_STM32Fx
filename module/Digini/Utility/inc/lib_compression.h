//-------------------------------------------------------------------------------------------------
//
//  File : lib_compression.h
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
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//extern unsigned  __block_1_base__;            // To be used as temporary holding data like decompression
//extern unsigned  __block_2_base__;            // if to be use by other process than decompression
//extern unsigned  __block_3_base__;            // should put mutex on this ressource
//extern unsigned  __block_4_base__;

//#define PREFIX_BASE_ADDRESS    (void*)&__block_1_base__
//#define APPEND_BASE_ADDRESS    (void*)&__block_2_base__
//#define DECODE_BASE_ADDRESS    (void*)&__block_3_base__
//#define RAW_INPUT_BASE_ADDRESS (void*)&__block_4_base__

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum Compression_e
{
    COMPX_COMPRESSION_NONE,
    COMPX_RLE_4,                                  // 4 bits per pixel  (font, or grayscale)
    COMPX_RLE_8,                                  // 8 bits per pixel  (font, or grayscale)
    COMPX_RLE_16,                                 // 16 bits per pixel (RGB565)
    COMPX_RLE_32,                                 // 32 bits per pixel (ARGB8888)
    COMPX_RLE_16_CLUT,                            // 16 bits per pixel (RGB565, with color table)
    COMPX_RLE_32_CLUT,                            // 32 bits per pixel (ARGB8888, with color table);
    COMPX_LZW_12,                                 // Good for all
    COMPX_LZW_13,                                 // Good for all
    COMPX_LZW_14,                                 // Good for all
    COMPX_LZW_15,                                 // Good for all
    COMPX_LZW_16,                                 // Good for all
    COMPX_JPEG,                                   // 16 bit Background, photo
    COMPX_PNG,                                    // 32 bit static, photo, logo
};

struct CompxWorkMem_t
{
    RawArray* pPrefix;
    RawArray* pAppend;
    RawArray* pDecode;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//
// Note(s)                  As this code need large amount of memory, it can only be use at startup
//                          to decompress data before the application start, because it use memory
//                          normally allocated to graphic.
//
//
//-------------------------------------------------------------------------------------------------

class DeCompression
{
    public:

                            DeCompression           (CompxWorkMem_t* pCompxWorkMem);

        size_t              Process                 (RawArray* pRawData, RawArray* pCompxData, size_t DataSize, uint8_t CompressionMethod);

    private:

        size_t              LZW_Method              (void);
        size_t              RLE_4_Method            (void);
        size_t              RLE_8_Method            (void);
        size_t              RLE_16_Method           (void);
        size_t              RLE_32_Method           (void);
        size_t              RLE_16_CLUT_Method      (void);
        size_t              RLE_32_CLUT_Method      (void);
        size_t              JPEG_Method             (void);

        // Sub function LZW
        uint32_t            LZW_InputCode           (void);
        void                LZW_DecodeArray         (uint32_t Code);


        size_t              m_DataSize;
        RawArray*           m_pCompxData;
        RawArray*           m_pRawData;

        // Variables for LZW
        uint8_t             m_Bits;
        uint8_t             m_HashingShift;
        uint32_t            m_MaxValue;
        uint32_t            m_MaxCode;
        uint32_t            m_DataOffset;
        uint32_t            m_TableSize;
        uint8_t             m_BitCount;
        uint32_t            m_BitBuffer;
        uint16_t            m_BytesLeftCounter;
        CompxWorkMem_t      m_WorkMem;
};

//-------------------------------------------------------------------------------------------------
