//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_ram_dbase.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_GRAFX
#include "lib_grafx.h"
#endif
#include "lib_macro.h"
#include <stdint.h>
#include "lib_class_ram_dbase.h"

//-------------------------------------------------------------------------------------------------

#if defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
//
//   Class: CRAM_DataBase
//
//
//   Description:   Class CRAM_DataBase
//
//-------------------------------------------------------------------------------------------------

// Create Quantity list for each record item
const uint16_t CRAM_DataBase::m_ItemsQTY[NB_RAM_DBASE_ITEMS_CONST]=                       // Array[THIS][]
{
  #ifdef RAM_DBASE_DEF
   #define X_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_QTY,
    RAM_DBASE_DEF
   #undef X_RAM_DBASE
  #endif

  #ifdef GFX_RAM_DBASE_DEF
   #define X_GFX_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_QTY,
    GFX_RAM_DBASE_DEF
   #undef X_GFX_RAM_DBASE
  #endif

  #ifdef NV_RAM_DBASE_DEF
   #define X_NV_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_QTY,
    NV_RAM_DBASE_DEF
   #undef X_NV_RAM_DBASE
  #endif
};

// Create SUB Quantity list for each record item
const uint16_t CRAM_DataBase::m_ItemsSubQTY[NB_RAM_DBASE_ITEMS_CONST] =                    // Array[][THIS]
{
  #ifdef RAM_DBASE_DEF
   #define X_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_SubQTY,
    RAM_DBASE_DEF
   #undef X_RAM_DBASE
  #endif

  #ifdef GFX_RAM_DBASE_DEF
   #define X_GFX_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_SubQTY,
    GFX_RAM_DBASE_DEF
   #undef X_GFX_RAM_DBASE
  #endif

  #ifdef NV_RAM_DBASE_DEF
   #define X_NV_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEMS_SubQTY,
    NV_RAM_DBASE_DEF
   #undef X_NV_RAM_DBASE
  #endif
};

// Create size list for each record item
const size_t CRAM_DataBase::m_ItemSize[NB_RAM_DBASE_ITEMS_CONST] =                         // sizeof()
{
  #ifdef RAM_DBASE_DEF
   #define X_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEM_SIZE,
    RAM_DBASE_DEF
   #undef X_RAM_DBASE
  #endif

  #ifdef GFX_RAM_DBASE_DEF
   #define X_GFX_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEM_SIZE,
    GFX_RAM_DBASE_DEF
   #undef X_GFX_RAM_DBASE
  #endif

  #ifdef NV_RAM_DBASE_DEF
   #define X_NV_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ITEM_SIZE,
    NV_RAM_DBASE_DEF
   #undef X_NV_RAM_DBASE
  #endif
};

//-------------------------------------------------------------------------------------------------
//
//   Function name: Initialize
//
//   Parameter(s):  void*           pConfig
//                  size_t          ObjectSize
//   Return:        SystemState_e   Always SYS_READY in this driver
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRAM_DataBase::Initialize(void* pConfig, size_t ObjectSize)
{
    uint16_t    i;
    size_t      RecordTotalSize;

    if(ObjectSize > sizeof(RAM_DBaseRegionPointer_t*)) return SYS_WRONG_SIZE;
    if(ObjectSize < sizeof(RAM_DBaseRegionPointer_t*)) return SYS_WRONG_SIZE;

    i = 0;

  #ifdef RAM_DBASE_DEF
    for(; i < ((END_RAM_INDEX - START_RAM_INDEX) - 1); i++)
    {
        m_ItemsPointer[i + 1] = nullptr;
    }

    if(((RAM_DBaseRegionPointer_t*)(pConfig))->pRam != nullptr)
    {
        this->SetDB_Address((void**)&((RAM_DBaseRegionPointer_t*)(pConfig))->pRam);
    }
  #endif

  #ifdef GFX_RAM_DBASE_DEF
    m_ItemsPointer[i] = ((RAM_DBaseRegionPointer_t*)(pConfig))->pGFX_Ram;

    for(; i < ((END_GFX_RAM_INDEX - START_GFX_RAM_INDEX) - 1); i++)
    {
        // Pre-calculate offset for each record
        RecordTotalSize = (size_t)m_ItemsQTY[i] * (size_t)m_ItemsSubQTY[i] * m_ItemSize[i];
        m_ItemsPointer[i + 1] = (uint8_t*)((size_t)(m_ItemsPointer[i]) + RecordTotalSize);
    }
  #endif

  #ifdef NV_RAM_DBASE_DEF
    m_ItemsPointer[i] = ((RAM_DBaseRegionPointer_t*)(pConfig))->pNV_Ram;
    for(; i < ((END_NV_RAM_INDEX - START_NV_RAM_INDEX) - 1); i++)
    {
        // Precalculate offset for each record
        RecordTotalSize = (size_t)m_ItemsQTY[i] * (size_t)m_ItemsSubQTY[i] * m_ItemSize[i];
        m_ItemsPointer[i + 1] = (uint8_t*)((size_t)(m_ItemsPointer[i]) + RecordTotalSize);
    }
  #endif

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
//   Description:   Get the record for RAM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRAM_DataBase::Get(void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    void*         pPointer;

    if((State = GetPointer(&pPointer, Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Record -= DBASE_INDEX_RAM_RANGE;
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
//   Description:   Set the record for RAM type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRAM_DataBase::Set(const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e   State;
    void*           pPointer;

    if((State = GetPointer(&pPointer, Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Record -= DBASE_INDEX_RAM_RANGE;
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
uint16_t CRAM_DataBase::GetDriverIndex(Range_e Range)
{
    if(Range == RANGE_MIN) return  START_RAM_DBASE + 1;
    return END_RAM_DBASE - 1;
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
SystemState_e CRAM_DataBase::GetSize(uint32_t* pSize, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e    State;

    Record -= DBASE_INDEX_RAM_RANGE;
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
SystemState_e CRAM_DataBase::GetPointer(void** pPointer, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    size_t       Offset;

    Record -= DBASE_INDEX_RAM_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Offset    = (size_t(Number) * size_t(m_ItemsSubQTY[Record])) + size_t(SubNumber);     // Calculate offset in items QTY
    Offset   *= m_ItemSize[Record];                                                       // Multiply by the size of the item
    Offset   += (size_t)m_ItemsPointer[Record];                                           // Add it to the real pointer value
    *pPointer = (void*)Offset;

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//   Function name: SetDB_Address
//
//   Parameter(s):  void**         pAddress
//   Return:        SystemState_e
//
//   Description:   Set the memory address for this RAM database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CRAM_DataBase::SetDB_Address(void** pAddress)
{
  #ifdef RAM_DBASE_DEF
    uint16_t i;
    size_t   RecordTotalSize;

    m_ItemsPointer[0] = (uint8_t*)*pAddress;
    LIB_AlignPointer(m_ItemsPointer[0]);

    for(i = 0; i < ((END_RAM_INDEX - START_RAM_INDEX) - 1); i++)
    {
        // Precalculate offset for each record
        RecordTotalSize = (size_t)m_ItemsQTY[i] * (size_t)m_ItemsSubQTY[i] * m_ItemSize[i];
        m_ItemsPointer[i + 1] = (uint8_t*)((size_t)(m_ItemsPointer[i]) + RecordTotalSize);
        LIB_AlignPointer(m_ItemsPointer[i + 1]);
    }

    *pAddress = (void*)m_ItemsPointer[i]; // Return the end address

    return SYS_READY;
  #else
    VAR_UNUSED(pAddress);
    return SYS_UNSUPPORTED_FEATURE;
  #endif
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
SystemState_e CRAM_DataBase::CheckRange(uint16_t Record, uint16_t Number, uint16_t SubNumber)
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

#endif // #if defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)
