//-------------------------------------------------------------------------------------------------
//
//  File : lib_fifo.cpp
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
#include "lib_fifo.h"

//-------------------------------------------------------------------------------------------------
//
//   Class: CFIFO
//
//   Description:   Class to handle CFIFO
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CFIFO
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the fifo
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CFIFO::CFIFO()
{
}

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CFIFO
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the fifo
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CFIFO::CFIFO(uint16_t Size)
{
    m_Size = Size;
    m_pBuffer = new uint8_t[m_Size];
    m_PushIndex = 0;
    m_PopIndex  = 0;
    m_Count = 0;
}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CFIFO
//
//   Parameter(s):
//   Return Value:
//
//   Description:    Deinitializes the fifo
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CFIFO::~CFIFO(void)
{
}

//-------------------------------------------------------------------------------------------------
//
//   Name:   IsEmpty
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CFIFO::IsEmpty(void)
{
    return (m_Count <= 0);
}

//-------------------------------------------------------------------------------------------------
//
//   Name:   IsFull
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CFIFO::IsFull(void)
{
    return (m_Count >= m_Size);
}

//-------------------------------------------------------------------------------------------------
//
//   Name:   Pop
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CFIFO::Pop(uint8_t* pData)
{
    // The FIFO is empty
    if (IsEmpty())
        return false;

    // Pop the data
    *pData = m_pBuffer[m_PopIndex];

    // Calculate the next pop index and update
    m_PopIndex = (m_PopIndex + 1) % m_Size;

    //Update count
    m_Count--;

    return true;
}

//-------------------------------------------------------------------------------------------------
//
//   Name:   Push
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
bool CFIFO::Push(uint8_t Data)
{
    // The FIFO is full
    if(IsFull())
        return false;

    //Push the data
    m_pBuffer[m_PushIndex] = Data;

    // Update the push index
    m_PushIndex = (m_PushIndex + 1) % m_Size;

    //Update count
    m_Count++;

    return true;
}


//-------------------------------------------------------------------------------------------------
//
//   Name:   Count
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t CFIFO::Count(void)
{
    return m_Count;
}


