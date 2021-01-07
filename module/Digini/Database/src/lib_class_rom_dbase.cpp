//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_rom_dbase.cpp
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
#include "lib_class_rom_dbase.h"
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------

#if defined(ROM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
//
//   Class: CROM_DataBase
//
//
//   Description:   Class CROM_DataBase
//
//-------------------------------------------------------------------------------------------------

// Create pointer list for each record item
const uint8_t* CROM_DataBase::m_ItemsPointer[NB_ROM_DBASE_ITEMS_CONST] =                                   // &Array[0][0]
{
  #define X_ROM_DBASE(ENUM_ID, ITEM_ADDRESS, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) (uint8_t*)ITEM_ADDRESS,
    ROM_DBASE_DEF
  #undef X_ROM_DBASE
};

// Create Quantity list for each record item
const uint16_t CROM_DataBase::m_ItemsQTY[NB_ROM_DBASE_ITEMS_CONST] =                                       // Array[THIS][]
{
  #define X_ROM_DBASE(ENUM_ID, ITEM_ADDRESS, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_QTY,
    ROM_DBASE_DEF
  #undef X_ROM_DBASE
};

// Create SUB Quantity list for each record item
const uint16_t CROM_DataBase::m_ItemsSubQTY[NB_ROM_DBASE_ITEMS_CONST] =                                    // Array[][THIS]
{
  #define X_ROM_DBASE(ENUM_ID, ITEM_ADDRESS, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_SubQTY,
    ROM_DBASE_DEF
  #undef X_ROM_DBASE
};

// Create size list for each record item
const size_t CROM_DataBase::m_ItemSize[NB_ROM_DBASE_ITEMS_CONST] =                                         // sizeof()
{
  #define X_ROM_DBASE(ENUM_ID, ITEM_ADDRESS, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEM_SIZE,
    ROM_DBASE_DEF
  #undef X_ROM_DBASE
};


//-------------------------------------------------------------------------------------------------
//
//   Function name: Initialize
//
//   Parameter(s):  void*           pConfig
//                  size_t          ObjectSize
//   Return:        SystemState_e   State
//
//   Description:   Initialize driver
//
//-------------------------------------------------------------------------------------------------
SystemState_e CROM_DataBase::Initialize(void* pConfig, size_t ObjectSize)
{
    // Nothing in this driver to init
    VAR_UNUSED(pConfig);
    VAR_UNUSED(ObjectSize);
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
//   Return:        SystemState_e   State
//
//   Description:   Get the record for ROM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CROM_DataBase::Get(void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;
    void*           pPointer;

    Record -= DBASE_INDEX_ROM_RANGE;
    if((State = GetPointer(&pPointer, Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    memcpy(pData, pPointer, m_ItemSize[Record]);

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
//   Description:   Set the record for ROM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CROM_DataBase::Set(const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;
    void*           pPointer;

    Record -= DBASE_INDEX_ROM_RANGE;
    if((State = GetPointer(&pPointer, Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    memcpy(pPointer, pData, m_ItemSize[Record]);

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetDriverIndex
//
//   Parameter(s):  Range_e     Range           RANGE_MIN or RANGE_MAX
//   Return:        uint16_t    Index
//
//   Description:   Return min or the maximum  for the range of index
//
//-------------------------------------------------------------------------------------------------
uint16_t CROM_DataBase::GetDriverIndex(Range_e Range)
{
    if(Range == RANGE_MIN) return START_ROM_DBASE + 1;
    return END_ROM_DBASE - 1;
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
SystemState_e CROM_DataBase::GetSize(uint32_t* pSize, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;

    Record -= DBASE_INDEX_ROM_RANGE;
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
SystemState_e CROM_DataBase::GetPointer(void** pPointer, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    size_t       Offset;

    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Offset    = (size_t(Number) * size_t(m_ItemsSubQTY[Record])) + size_t(SubNumber);     // Calculate offset in items QTY
    Offset   *= m_ItemSize[Record];                                                       // Multiply bu the size of the item
    Offset   += (size_t)m_ItemsPointer[Record];                                           // Add it to the real pointer value
    *pPointer = (void*)Offset;

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
//   Description:   Check the range provided if not out of bound
//
//-------------------------------------------------------------------------------------------------
SystemState_e CROM_DataBase::CheckRange(uint16_t Record, uint16_t Number, uint16_t SubNumber)
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

#endif // #if defined(ROM_DBASE_DEF)


