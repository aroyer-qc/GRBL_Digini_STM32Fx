//-------------------------------------------------------------------------------------------------
//
//  File : lib_compression.cpp
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
#include "lib_compression.h"
#include "jpeg.h"
#include "picopng.h"

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
DeCompression::DeCompression(CompxWorkMem_t* pCompxWorkMem)
{
    memcpy(&m_WorkMem, pCompxWorkMem, sizeof(CompxWorkMem_t));
}

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):       JPEG decoder do not return the decompressed size so we provide the size
//                  with DataSize. The information is base on the ImageInfo_t for this image.
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::Process(RawArray* pRawData, RawArray* pCompxData, size_t DataSize, uint8_t CompressionMethod)
{
    size_t DecompressedSize;

    DecompressedSize = 0;
    m_DataSize   = DataSize;
    m_pCompxData = pCompxData;
    m_pRawData   = pRawData;

    switch(CompressionMethod)
    {
        case COMPX_RLE_4:       DecompressedSize = this->RLE_4_Method       (); break;
        case COMPX_RLE_8:       DecompressedSize = this->RLE_8_Method       (); break;
        case COMPX_RLE_16:      DecompressedSize = this->RLE_16_Method      (); break;
        case COMPX_RLE_32:      DecompressedSize = this->RLE_32_Method      (); break;
        case COMPX_RLE_16_CLUT: DecompressedSize = this->RLE_16_CLUT_Method (); break;
        case COMPX_RLE_32_CLUT: DecompressedSize = this->RLE_32_CLUT_Method (); break;

        case COMPX_LZW_12:
        case COMPX_LZW_13:
        case COMPX_LZW_14:
        case COMPX_LZW_15:
        case COMPX_LZW_16:
        {
            m_Bits           = (CompressionMethod - COMPX_LZW_12) + 12;
            DecompressedSize = this->LZW_Method();
            break;
        }
        case COMPX_JPEG:
        {
            JPEG_Decode(pCompxData->data(), pRawData->data(), &DecompressedSize);
            break;
        }
        case COMPX_PNG:
        {
            PNG_Info_t Info = {0};

            Info.pOut                = pRawData->data();
            Info.pIn                 = pCompxData->data();
            Info.DataSize            = DataSize;
            Info.IDAT.data.u8        = m_WorkMem.pAppend->data();    // Append buffer from lzw for PNG chunk data
            Info.IDAT.allocsize      = 522240;
            Info.ScanLines.data.u8   = m_WorkMem.pPrefix->data();    // Prefix buffer from lzw for PNG chunk data
            Info.ScanLines.allocsize = 522240;
            Info.Image.data.u8       = m_WorkMem.pDecode->data();    // Prefix buffer from lzw for PNG chunk data
            Info.Image.allocsize     = 522240;
            DecompressedSize         = PNG_decode(&Info);
            break;
        }
        case COMPX_COMPRESSION_NONE:
        {
            for(size_t i = 0; i < m_DataSize; i++)
            {
                m_pRawData->append(m_pCompxData->at(i));
            }
            DecompressedSize = m_DataSize;
            break;
        }
    }

    return DecompressedSize;
}

//-------------------------------------------------------------------------------------------------
//
//  ****  ****  * *   *   *  ***** *****
//  *   * *   * * *   *  * *   *   *
//  ****  ****  * *   * *   *  *   ****
//  *     *  *  *  * *  *****  *   *
//  *     *   * *   *   *   *  *   *****
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_4_Method(void)
{
    int      LenghtEncodeCount;
    uint8_t  ReadValue;
    uint32_t DataOffset = 0;

    while(m_DataSize > 0)
    {
        ReadValue = m_pCompxData->at(DataOffset++);

        LenghtEncodeCount = (ReadValue >> 4) + 1;
        ReadValue        &= 0x0F;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_8_Method(void)
{
    int     LenghtEncodeCount;
    uint8_t ReadValue;
    uint32_t DataOffset = 0;

    m_DataSize >>= 1;

    while(m_DataSize > 0)
    {
        LenghtEncodeCount = m_pCompxData->at(DataOffset++) + 1;
        ReadValue         = m_pCompxData->at(DataOffset++);

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_16_Method(void)
{
    int      LenghtEncodeCount;
    uint16_t ReadValue;
    uint32_t DataOffset = 0;

    m_DataSize /= 3;

    while(m_DataSize > 0)
    {
        LenghtEncodeCount = m_pCompxData->at(DataOffset++) + 1;
        ReadValue         = m_pCompxData->uint16_At(DataOffset);
        DataOffset       += 2;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_32_Method(void)
{
    int      LenghtEncodeCount;
    uint32_t ReadValue;
    uint32_t DataOffset = 0;

    m_DataSize /= 5;

    while(m_DataSize > 0)
    {
        LenghtEncodeCount = m_pCompxData->at(DataOffset++) + 1;
        ReadValue         = m_pCompxData->uint32_At(DataOffset);
        DataOffset       += 4;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_16_CLUT_Method(void)
{
    int      ReadIndex;
    int      LenghtEncodeCount;
    int      CLUT_Offset;
    uint16_t ReadValue;
    uint32_t DataOffset = 0;

    CLUT_Offset   = DataOffset + 1;                                         // Beginning of the CLUT
    DataOffset   += ((((int)m_pCompxData->at(DataOffset) + 1) * 2) + 1);    // Beginning of the data
    m_DataSize--;
    m_DataSize   -= (DataOffset - CLUT_Offset);
    m_DataSize  >>= 1;

    while(m_DataSize > 0)
    {
        LenghtEncodeCount = m_pCompxData->at(DataOffset++) + 1;
        ReadIndex         = m_pCompxData->at(DataOffset++);

        ReadValue = m_pCompxData->uint16_At(CLUT_Offset + (ReadIndex * 2));

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::RLE_32_CLUT_Method(void)
{
    int      ReadIndex;
    int      LenghtEncodeCount;
    int      CLUT_Offset;
    uint32_t ReadValue;
    uint32_t DataOffset = 0;

    CLUT_Offset   = DataOffset + 1;                                         // Beginning of the CLUT
    DataOffset   += ((((int)m_pCompxData->at(DataOffset) + 1) * 4) + 1);    // Beginning of the data
    m_DataSize--;
    m_DataSize   -= (DataOffset - CLUT_Offset);
    m_DataSize  >>= 1;

    while(m_DataSize > 0)
    {
        LenghtEncodeCount = m_pCompxData->at(DataOffset++) + 1;
        ReadIndex         = m_pCompxData->at(DataOffset++);

        ReadValue = m_pCompxData->uint32_At(CLUT_Offset + (ReadIndex * 4));

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            m_pRawData->append(ReadValue);
        }
        m_DataSize--;
    }
    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
size_t DeCompression::LZW_Method(void)
{
    uint32_t   NextCode;
    uint32_t   NewCode;
    uint32_t   OldCode;
    uint32_t   Code;

    m_HashingShift     = m_Bits - 8;
    m_MaxValue         = (1 << m_Bits) - 1;
    m_MaxCode          = m_MaxValue - 1;
    m_BytesLeftCounter = 0;

    NextCode     = 256;                                             // This is the next available code to define
    m_BitCount   = 0;
    m_BitBuffer  = 0L;
    m_DataOffset = 0L;

    OldCode = this->LZW_InputCode();                                // Read in the first code, initialize the
    Code    = OldCode;                                              // character variable, and send the first
    m_pRawData->append(uint8_t(OldCode));                           // code to the output vector

    // This is the main expansion loop.  It reads in characters from the LZW file
    // until it sees the special code used to inidicate the end of the data.

    while((NewCode = this->LZW_InputCode()) != (m_MaxValue))
    {
        // This code checks for the special ARRAY+CODE+ARRAY+CODE+ARRAY
        // case which generates an undefined code.  It handles it by decoding
        // the last code, and adding a single character to the end of the decode string.
        if(NewCode >= NextCode)
        {
            m_WorkMem.pDecode->append(Code);
            this->LZW_DecodeArray(OldCode);
        }
        // Otherwise we do a straight decode of the new code.
        else
        {
            this->LZW_DecodeArray(NewCode);
        }

        // Now we output the decoded string in reverse order.
        Code = m_WorkMem.pDecode->uint32_Last();
        while(m_WorkMem.pDecode->size() != 0)
        {
//            uint8_t Value = m_WorkMem.pDecode->uint32_TakeLast();
            uint8_t Value = m_WorkMem.pDecode->uint32_TakeLast();
            m_pRawData->append(Value);
        }

        // Finally, if possible, add a new code to the string table.
        if(NextCode <= m_MaxCode)
        {
            m_WorkMem.pPrefix->replace(NextCode * sizeof(uint32_t), OldCode);
            m_WorkMem.pAppend->replace(NextCode * sizeof(uint32_t), Code);
            NextCode++;
        }
        OldCode = NewCode;
    }

    return m_pRawData->size();
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void DeCompression::LZW_DecodeArray(uint32_t Code)
{
    uint32_t i = 0;

    while(Code > 255)
    {
        m_WorkMem.pDecode->append(m_WorkMem.pAppend->uint32_At(Code * sizeof(uint32_t)));
        Code = m_WorkMem.pPrefix->uint32_At(Code * sizeof(uint32_t));

        if(i++ >= m_MaxCode)
        {
            //ERROR  //exit(-3);
            __asm("nop");
        }
    }
    m_WorkMem.pDecode->append(Code);
}


//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint32_t DeCompression::LZW_InputCode(void)
{
    uint32_t Value;

    while( m_BitCount <= 24)
    {
        m_BitBuffer |= ((uint32_t)m_pCompxData->at(m_DataOffset) << (24 -  m_BitCount));
        m_DataOffset++;
        m_BitCount += 8;
    }

    Value = m_BitBuffer >> (32 - m_Bits);
     m_BitBuffer <<= m_Bits;
     m_BitCount   -= m_Bits;

    return Value;
}

//-------------------------------------------------------------------------------------------------
