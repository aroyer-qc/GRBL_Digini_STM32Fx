//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_qspi_dbase.cpp
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
#include <string.h>
#include "lib_class_qspi_dbase.h"

//-------------------------------------------------------------------------------------------------

#if defined(QSPI_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
//
//   Class: CQSPI_DataBase
//
//
//   Description:   Class CQSPI_DataBase
//
//-------------------------------------------------------------------------------------------------

// Create Quantity list for each record item
const uint16_t CQSPI_DataBase::m_ItemsQTY[NB_QSPI_DBASE_ITEMS_CONST] =                          // Array[THIS][]
{
  #define X_QSPI_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_QTY,
    QSPI_DBASE_DEF
  #undef X_QSPI_DBASE
};

// Create SUB Quantity list for each record item
const uint16_t CQSPI_DataBase::m_ItemsSubQTY[NB_QSPI_DBASE_ITEMS_CONST] =                       // Array[][THIS]
{
  #define X_QSPI_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_SubQTY,
    QSPI_DBASE_DEF
  #undef X_QSPI_DBASE
};

// Create size list for each record item
const size_t CQSPI_DataBase::m_ItemSize[NB_EEPROM_DBASE_ITEMS_CONST] =                          // sizeof()
{
  #define X_QSPI_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEM_SIZE,
    QSPI_DBASE_DEF
  #undef X_QSPI_DBASE
};


//-------------------------------------------------------------------------------------------------
//
//   Function name: Initialize
//
//   Parameter(s):  size_t          ObjectSize
//   Return:        SystemState_e   Always READY in this driver
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::Initialize(void* pConfig, size_t ObjectSize)
{
    uint16_t    i;
    size_t      RecordTotalSize;

    if(ObjectSize > sizeof(size_t)) return SYS_WRONG_SIZE;
    if(ObjectSize < sizeof(size_t)) return SYS_WRONG_SIZE;

    m_pQuadSPI        = ((sQuadSPI_DBaseInfo*)(pConfig))->pQuadSPI;
    m_ItemsAddress[0] = ((sQuadSPI_DBaseInfo*)(pConfig))->Address;

    for(i = 0; i < NB_QSPI_DBASE_ITEMS_CONST; i++)
    {
        // Precalculate offset for each record
        RecordTotalSize       = (size_t)m_ItemsQTY[i] * (size_t)m_ItemsSubQTY[i] * m_ItemSize[i];
        m_ItemsAddress[i + 1] = ((size_t)(m_ItemsAddress[i]) + RecordTotalSize);
    }

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Get
//
//   Parameter(s):  void*           pData
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e    State
//
//   Description:   Get the record for EEPROM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::Get(void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;
    uint32_t        Address;

    Record -= DBASE_INDEX_QSPI_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Address = GetAddress(Record, Number, SubNumber);
    m_pEEprom->Read(Address, pData, m_ItemSize[Record]);

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Set
//
//   Parameter(s):  const void*     pData
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e    State
//
//   Description:   Set the record for EEPROM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::Set(const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;
    uint32_t        Address;

    Record -= DBASE_INDEX_QSPI_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Address = GetAddress(Record, Number, SubNumber);
    m_pEEprom->Write(Address, pData, m_ItemSize[Record]);

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetDriverIndex
//
//   Parameter(s):  Range_e     Range           RANGE_MIN or RANGE_MAX
//   Return:        uint16_t    Index
//
//   Description:   Return min or the maximum for the range of index
//
//-------------------------------------------------------------------------------------------------
uint16_t CQSPI_DataBase::GetDriverIndex(Range_e Range)
{
    if(Range == RANGE_MIN) return  START_QSPI_DBASE + 1;
    return END_QSPI_DBASE - 1;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetSize
//
//   Parameter(s):  uint16_t        Record
//                  uint32_t*       pSize
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        uint32_t*       SystemState_e
//
//   Description:   Return the size of record
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::GetSize(uint32_t* pSize, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;

    Record -= DBASE_INDEX_QSPI_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    *pSize = m_ItemSize[Record];
    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetPointer
//
//   Parameter(s):  void**          pPointer
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e
//
//   Description:   Return the memory address from item information
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::GetPointer(void** pPointer, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    *pPointer = NULL;
    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: CheckRange
//
//   Parameter(s):  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e    State
//
//   Description:   Check the range provided if it is not out of bound
//
//-------------------------------------------------------------------------------------------------
SystemState_e CQSPI_DataBase::CheckRange(uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    if(Number < m_ItemsQTY[Record])
    {
        if(SubNumber < m_ItemsSubQTY[Record])
        {
            return SYS_READY;
        }

    }
    return SYS_OUT_OF_RANGE;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetAddress
//
//   Parameter(s):  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        uint32_t        Address
//
//   Description:   return the memory address from item information
//
//-------------------------------------------------------------------------------------------------
uint32_t CQSPI_DataBase::GetAddress(uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    uint32_t Offset;

    Offset  = (uint32_t(Number) * uint32_t(m_ItemsSubQTY[Record])) + uint32_t(SubNumber);       // Calculate offset in items QTY
    Offset *= m_ItemSize[Record];                                                               // Multiply bu the size of the item
    Offset += (uint32_t)m_ItemsAddress[Record];                                                 // Add it to the real pointer value

    return Offset;
}

//-------------------------------------------------------------------------------------------------

#endif // #if defined(SQPI_DBASE_DEF)
