//-------------------------------------------------------------------------------------------------
//
//  File :  lib_raw_array.cpp
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
//
//  Note :  This class does not check for boundary, it take what ever it need.
//          it is use to hold raw data loaded in specially reserved memory
//          Really basic.
//          All Index are in uint8_t position.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------
//
//   Constructor
//
//-------------------------------------------------------------------------------------------------

RawArray::RawArray(void)
{
    m_Size    = 0;
    m_pBuffer = nullptr;
}

RawArray::RawArray(void* pBuffer)
{
    m_Size    = 0;
    m_pBuffer = (uint8_t*)pBuffer;
}

RawArray::RawArray(void* pBuffer, size_t Size)
{
    m_Size    = Size;
    m_pBuffer = (uint8_t*)pBuffer;
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor
//
//-------------------------------------------------------------------------------------------------

RawArray::~RawArray(void)
{
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      append
//
//   Description:   Append an item at the end of the array.
//
//-------------------------------------------------------------------------------------------------

void RawArray::append(uint8_t data)
{
    uint32_t Offset = m_Size;
    m_Size++;
    this->replace(Offset, data);
}

void RawArray::append(uint16_t data)
{
    uint32_t Offset = m_Size;
    m_Size += 2;
    this->replace(Offset, data);
}

void RawArray::append(uint32_t data)
{
    uint32_t Offset = m_Size;
    m_Size += 4;
    this->replace(Offset, data);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      at
//
//   Description:   Get item in array from specified index.
//
//-------------------------------------------------------------------------------------------------

uint8_t RawArray::at(uint32_t Index)
{
    return *(m_pBuffer + Index);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      clear
//
//   Description:   Remove all array's contents.
//
//-------------------------------------------------------------------------------------------------

void RawArray::clear(void)
{
    m_Size = 0;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      data
//
//   Description:   Returns array's pointer.
//
//-------------------------------------------------------------------------------------------------

uint8_t* RawArray::data(void)
{
    return m_pBuffer;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      insert
//
//   Description:   Insert an item in array at specified index.
//
//-------------------------------------------------------------------------------------------------

void RawArray::insert(uint32_t Index, uint8_t data)
{
    m_Size++;

    for(uint32_t i = m_Size; i > (Index + 1); i--)
    {
        *(m_pBuffer + i - 1) = *(m_pBuffer + i - 2);
    }
    *(m_pBuffer + Index) = data;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      fill
//
//   Description:   Fills array with specified data.
//
//-------------------------------------------------------------------------------------------------
void RawArray::fill(uint8_t data)
{
    for(size_t i = 0; i < m_Size; i++)
    {
        *(m_pBuffer + i) = data;
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      last
//
//   Description:   Gets array's last value from Array.
//
//-------------------------------------------------------------------------------------------------
uint8_t RawArray::last(void)
{
    return this->at(m_Size - 1);
}

uint16_t RawArray::uint16_Last(void)
{

    return this->uint16_At(m_Size - 2);
}

uint32_t RawArray::uint32_Last(void)
{
    return this->uint32_At(m_Size - 4);
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      remove
//
//   Description:   Remove an item in array at specified index.
//
//-------------------------------------------------------------------------------------------------

void RawArray::remove(uint32_t Index)
{
    for(uint32_t i = Index; i < (m_Size - 1); i++)
    {
        *(m_pBuffer + i) = *(m_pBuffer + i + 1);
    }
    m_Size--;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      replace
//
//   Description:   replace item in array at specified index.
//
//-------------------------------------------------------------------------------------------------
bool RawArray::replace(uint32_t Index, uint8_t data)
{
    bool bStatus = false;

    if(Index < m_Size)
    {
        *(m_pBuffer + Index) = data;
        bStatus              = true;
    }
    return bStatus;
}

bool RawArray::replace(uint32_t Index, uint16_t data)
{
    bool bStatus = false;

    if((Index + 1) < m_Size)
    {
        this->replace(Index++, (uint8_t)(data >> 8));
        this->replace(Index++, (uint8_t)data);
    }
    return bStatus;
}

bool RawArray::replace(uint32_t Index, uint32_t data)
{
    bool bStatus = false;

    if((Index + 3) < m_Size)
    {
        this->replace(Index++, (uint8_t)(data >> 24));
        this->replace(Index++, (uint8_t)(data >> 16));
        this->replace(Index++, (uint8_t)(data >> 8));
        this->replace(Index++, (uint8_t)data);
    }
    return bStatus;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      resize
//
//   Description:   Sets array's new size.
//
//-------------------------------------------------------------------------------------------------
void RawArray::resize(size_t Size)
{
    m_Size = Size;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      setBuffer
//
//   Description:   Set the buffer pointer
//
//-------------------------------------------------------------------------------------------------
void RawArray::setBuffer(uint8_t* pBuffer)
{
    m_pBuffer = pBuffer;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      size
//
//   Description:   Gets array's current size.
//
//-------------------------------------------------------------------------------------------------
size_t RawArray::size(void)
{
    return m_Size;
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      takeLast
//
//   Description:   Gets array's last value and remove it from Array.
//
//-------------------------------------------------------------------------------------------------

uint8_t RawArray::takeLast(void)
{
    uint8_t data;

    data = this->last();
    m_Size--;

    return data;
}

uint16_t RawArray::uint16_TakeLast(void)
{
    uint16_t data;

    data = this->uint16_Last();
    m_Size -= 2;

    return data;
}

uint32_t RawArray::uint32_TakeLast(void)
{
    uint32_t data;

    data = uint32_Last();
    m_Size -= 4;

    return data;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      uint16_At
//
//   Description:   return a uint16_t at the index
//
//-------------------------------------------------------------------------------------------------
uint16_t RawArray::uint16_At(uint32_t Index)
{
    uint16_t Value;

    Index += (uint32_t)m_pBuffer;

    Value  = ((uint16_t)(*(uint8_t*)Index) << 8);
    Index++;
    Value |=  (uint16_t)(*(uint8_t*)Index);

    return Value;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      uint32_At
//
//   Description:   return a uint32_t at the index
//
//-------------------------------------------------------------------------------------------------

uint32_t RawArray::uint32_At(uint32_t Index)
{
    uint32_t Value;

    Index += (uint32_t)m_pBuffer;

    Value  = ((uint32_t)(*(uint8_t*)Index) << 24);
    Index++;
    Value |= ((uint32_t)(*(uint8_t*)Index) << 16);
    Index++;
    Value |= ((uint32_t)(*(uint8_t*)Index) << 8);
    Index++;
    Value |=  (uint32_t)(*(uint8_t*)Index);

    return Value;
}





