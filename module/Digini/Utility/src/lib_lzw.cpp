//-------------------------------------------------------------------------------------------------
//
//  File :  lib_lzw.cpp
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

//-------------------------------------------------------------------------------------------------

lzw::lzw(uint8_t Bits, CompressWorkMem_t WorkingMemory)
{
    // TODO (alain) make this flexible and setting for users

    m_pPrefixCode  = new RawArray(PREFIX_BASE_ADDRESS);
    m_pPrefixCode->resize(524288U);
    m_pAppendCode  = new RawArray(APPEND_BASE_ADDRESS);
    m_pAppendCode->resize(524288U);
    m_pDecodeStack = new RawArray(DECODE_BASE_ADDRESS);

    m_Bits         = Bits;
    m_HashingShift = Bits - 8;
    m_MaxValue     = (1 << Bits) - 1;
    m_MaxCode      = m_MaxValue - 1;

    m_BytesLeftCounter = 0;
}

//-------------------------------------------------------------------------------------------------

lzw::~lzw()
{
}

//-------------------------------------------------------------------------------------------------

size_t lzw::DeCompress(size_t (*InputRead)(), size_t (*OutputWrite)(), size_t DataSize)
// size_t lzw::DeCompress(RawArray* pRawData, RawArray* pCompxData)
{
    uint32_t   NextCode;
    uint32_t   NewCode;
    uint32_t   OldCode;
    uint32_t   Code;
    uint32_t   DataOffset;

    NextCode     = 256;                                             // This is the next available code to define
    m_BitCount   = 0;
    m_BitBuffer  = 0;
    DataOffset   = 0;

    OldCode = InputCode(pCompxData, &DataOffset);                   // Read in the first code, initialize the
    Code    = OldCode;                                              // character variable, and send the first
    pRawData->append(uint8_t(OldCode));                             // code to the output vector

    // This is the main expansion loop.  It reads in characters from the LZW file
    // until it sees the special code used to inidicate the end of the data.

    while((NewCode = InputCode(pCompxData, &DataOffset)) != (m_MaxValue))
    {
        // This code checks for the special ARRAY+CODE+ARRAY+CODE+ARRAY
        // case which generates an undefined code.  It handles it by decoding
        // the last code, and adding a single character to the end of the decode string.
        if(NewCode >= NextCode)
        {

            m_pDecodeStack->append(Code);
            DecodeArray(OldCode);
        }
        // Otherwise we do a straight decode of the new code.
        else
        {
            DecodeArray(NewCode);
        }

        // Now we output the decoded string in reverse order.
        Code = m_pDecodeStack->uint32_Last();
        while(m_pDecodeStack->size() != 0)
        {
            uint8_t Value = m_pDecodeStack->uint32_TakeLast();
            pRawData->append(Value);
        }

        // Finally, if possible, add a new code to the string table.
        if(NextCode <= m_MaxCode)
        {
            m_pPrefixCode->replace(NextCode * sizeof(uint32_t), OldCode);
            m_pAppendCode->replace(NextCode * sizeof(uint32_t), Code);
            NextCode++;
        }
        OldCode = NewCode;
    }

    return pRawData->size();
}

//-------------------------------------------------------------------------------------------------

void lzw::DecodeArray(uint32_t Code)
{
    uint32_t i = 0;

    while(Code > 255)
    {
        m_pDecodeStack->append(m_pAppendCode->uint32_At(Code * sizeof(uint32_t)));
        Code = m_pPrefixCode->uint32_At(Code * sizeof(uint32_t));

        if(i++ >= m_MaxCode)
        {
            //ERROR  //exit(-3);
        }
    }
    m_pDecodeStack->append(Code);
}

//-------------------------------------------------------------------------------------------------

uint32_t lzw::InputCode(RawArray* pCompxData, uint32_t* pIndex)
{
    uint32_t Value;

    while( m_BitCount <= 24)
    {
        // ****************************************************************************************
        // NEW CODE
        static  // TODO put in class variable

        if(m_BytesLeftCounter == 0)
        {
            this->ReadFunction();
            m_BytesLeftCounter = SIZE_DEFINE_BY_CALLBACK_METHOD  // TODO put the right variable here
        }



        // ****************************************************************************************
        // OLD CODE
        // m_BitBuffer |= ((uint32_t)pCompxData->at(*pIndex) << (24 -  m_BitCount));
        // (*pIndex)++;
        // ****************************************************************************************

        m_BitCount += 8;
    }

    Value = (uint32_t)( m_BitBuffer >> (32 - m_Bits));
     m_BitBuffer <<= m_Bits;
     m_BitCount   -= m_Bits;

    return Value;
}
