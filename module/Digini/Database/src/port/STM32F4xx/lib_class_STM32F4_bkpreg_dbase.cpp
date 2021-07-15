//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_bkpreg_dbase.cpp
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include <stdint.h>
#include "lib_class_STM32F4_bkpreg_dbase.h"
#include "lib_class_STM32F4_rtc.h"
#include "lib_macro.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if (USE_RTC_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
//
//   Class: CBKPREG_DataBase
//
//
//   Description:   Class CBKPREG_DataBase
//
//-------------------------------------------------------------------------------------------------

// Create Quantity list for each record item
const uint8_t CBKPREG_DataBase::m_ItemsQTY[NB_BKPREG_DBASE_ITEMS_CONST] =                                       // Array[THIS][]
{
  #define X_BKPREG_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY) ITEMS_QTY,
    BKPREG_DBASE_DEF
  #undef X_BKPREG_DBASE
};

// Create SUB Quantity list for each record item
const uint8_t CBKPREG_DataBase::m_ItemsSubQTY[NB_BKPREG_DBASE_ITEMS_CONST] =                                    // Array[][THIS]
{
  #define X_BKPREG_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY) ITEMS_SubQTY,
    BKPREG_DBASE_DEF
  #undef X_BKPREG_DBASE
};

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CBKPREG_DataBase
//
//   Parameter(s):  CRTC* pCRTC         Pointer on the CRTC class to access RTC Backup register
//
//   Description:   Initializes the database
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CBKPREG_DataBase::CBKPREG_DataBase(CRTC* pRTC)
{
    m_pRTC = pRTC;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Initialize
//
//   Parameter(s):  void*           pConfig
//                  size_t          ObjectSize
//   Return:        SystemState_e    State
//
//   Description:   Initialize driver
//
//-------------------------------------------------------------------------------------------------
SystemState_e CBKPREG_DataBase::Initialize(void* pConfig, size_t ObjectSize)
{
    uint16_t    i;
    uint8_t     Index;

    VAR_UNUSED(pConfig);
    VAR_UNUSED(ObjectSize);

    if(NB_BKPREG_DBASE_ITEMS_CONST > 20) return SYS_OUT_OF_RANGE;

    Index           = 0;
    m_ItemsIndex[0] = Index;
    for(i = 0; i < NB_BKPREG_DBASE_ITEMS_CONST; i++)
    {
        // Precalculate offset for each record
        Index += (m_ItemsQTY[i] * m_ItemsSubQTY[i]);
        m_ItemsIndex[i + 1] = Index;
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
//   Description:   Get the record for backup register type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CBKPREG_DataBase::Get(void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    uint8_t      Index;

    Record -= DBASE_INDEX_BKPREG_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Index = GetIndex(Record, Number, SubNumber);
    *(uint32_t*)pData = m_pRTC->GetBackupRegister(Index);

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
//   Description:   Set the record for backup register type database
//
//-------------------------------------------------------------------------------------------------
SystemState_e CBKPREG_DataBase::Set(const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    uint8_t      Index;

    Record -= DBASE_INDEX_BKPREG_RANGE;
    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Index = GetIndex(Record, Number, SubNumber);
    m_pRTC->SetBackupRegister(Index, *(uint32_t*)pData);

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
uint16_t CBKPREG_DataBase::GetDriverIndex(Range_e Range)
{
    if(Range == RANGE_MIN) return START_BKPREG_INDEX + 1;
    return END_BKPREG_INDEX - 1;
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
SystemState_e CBKPREG_DataBase::GetSize(uint32_t* pSize, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    *pSize = sizeof(uint32_t);
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
SystemState_e CBKPREG_DataBase::GetPointer(void** pPointer, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    SystemState_e State;
    size_t       Offset;

    if((State = CheckRange(Record, Number, SubNumber)) != SYS_READY)
    {
        return State;
    }

    Offset    = BACKUP_REGISTER;
    Offset   += (GetIndex(Record,  Number, SubNumber) * sizeof(uint32_t));
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
SystemState_e CBKPREG_DataBase::CheckRange(uint16_t Record, uint16_t Number, uint16_t SubNumber)
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
//   Function name: GetIndex
//
//   Parameter(s):  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        uint8_t         Index
//
//   Description:   Return the index from item information
//
//-------------------------------------------------------------------------------------------------
uint8_t CBKPREG_DataBase::GetIndex(uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    uint8_t Index;

    Index  = uint8_t(Number) * uint8_t(m_ItemsSubQTY[Record]) + uint8_t(SubNumber);     // Calculate offset in items QTY
    Index += m_ItemsIndex[Record];                                                      // Add it to the real index value

    return Index;
}

//-------------------------------------------------------------------------------------------------

#endif // USE_RTC_DRIVER == DEF_ENABLED
