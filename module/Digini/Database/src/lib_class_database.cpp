//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_database.cpp
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
#define LIB_CLASS_DATABASE_GLOBAL
#include "lib_class_database.h"
#undef  LIB_CLASS_DATABASE_GLOBAL
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------
//
//   Class: CDataBase
//
//
//   Description:   Class CDataBase
//
//-------------------------------------------------------------------------------------------------

#ifdef DBASE_DEF

    const uint16_t CDataBase::m_RamRecord[NB_DBASE_ITEMS_CONST] =                                         // Array[THIS][]
    {
      #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, MINIMUM, MAXIMUM, DEFAULT) RAM_RECORD,
        DBASE_DEF
      #undef X_DBASE
    };

    const uint16_t CDataBase::m_RomRecord[NB_DBASE_ITEMS_CONST] =                                         // Array[][THIS]
    {
      #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, MINIMUM, MAXIMUM, DEFAULT) ROM_RECORD,
        DBASE_DEF
      #undef X_DBASE
    };

    const uint32_t CDataBase::m_Minimum[NB_DBASE_ITEMS_CONST] =                                          // sizeof()
    {
      #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, MINIMUM, MAXIMUM, DEFAULT) INTERVAL,
        DBASE_DEF
      #undef X_DBASE
    };

    const uint32_t CDataBase::m_Maximum[NB_DBASE_ITEMS_CONST] =                                         // sizeof()
    {
      #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, MINIMUM, MAXIMUM, DEFAULT) START_TIME,
        DBASE_DEF
      #undef X_DBASE
    };

    const uint32_t CDataBase::m_Default[NB_DBASE_ITEMS_CONST] =                                         // sizeof()
    {
      #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, MINIMUM, MAXIMUM, DEFAULT) START_TIME,
        DBASE_DEF
      #undef X_DBASE
    };

#endif


//-------------------------------------------------------------------------------------------------
//
//   Class: CDataBaseInterface
//
//
//   Description:   Class CDataBaseInterface
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CDataBaseInterface
//
//   Parameter(s):  None
//
//   Description:   Initializes the database
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CDataBaseInterface::CDataBaseInterface(void)
{
     // nothing to do yet
}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:    CDataBaseInterface
//
//   Description:   Free up any resources if any
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CDataBaseInterface::~CDataBaseInterface(void)
{
    // nothing to do yet
}



//-------------------------------------------------------------------------------------------------
//
//   Class: CDataBase
//
//
//   Description:   Class CDataBase
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CDataBase
//
//   Parameter(s):  None
//
//   Description:   Initializes the database
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CDataBase::CDataBase(void)
{
    // Make sure pool is empty
    for(int i = 0; i < DBASE_INTERFACE_POOL; i++)
    {
        m_pDriver[i] = nullptr;
    }

    // Reset Pool Count, no interface present
    m_PoolCount = 0;
}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:    CDataBase
//
//   Description:   Free up any resources if any
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CDataBase::~CDataBase(void)
{
    // nothing to do yet
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Init
//
//   Parameter(s):
//   Return:
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::Initialize(void)
{
   #ifdef DBASE_DEF
    // Reinit all system database flag (RAM + ROM)
    for(uint16_t i = 0; i < NB_DBASE_ITEMS_CONST; i++)
    {
        m_IsItInSync[i] = false;

    }
   #endif

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: RegisterDriver
//
//   Parameter(s):
//   Return:
//
//   Description:
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::RegisterDriver(CDataBaseInterface* DataBaseInterface)
{
    if(m_PoolCount < DBASE_INTERFACE_POOL)
    {
        m_pDriver[m_PoolCount] = DataBaseInterface;
        m_PoolCount++;
        return SYS_READY;
    }
    return SYS_POOL_FULL;
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
//   Description:   Get the database items
//
//   Notes:         SYS_NO_DRIVER = No driver found for the record number, so it is a range check at
//                              the same time.
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::Get(void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    CDataBaseInterface* pDriver;

   #ifdef DBASE_DEF
    // Translate for System database (RAM + ROM)
    if(Record < END_DBASE_INDEX)
    {
        Record = m_RamRecord[Record];
    }
   #endif

    pDriver = GetDriver(Record);
    if(pDriver != nullptr)
    {
        return pDriver->Get(pData, Record, Number, SubNumber);
    }

    return SYS_NO_DRIVER;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: Set
//
//   Parameter(s):  void*           pData
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e    State
//
//   Description:   Set the database items
//
//   Notes:         SYS_NO_DRIVER = No driver found for the record number, so it is a range check
//                                  at the same time.
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::Set(const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    CDataBaseInterface* pDriver;

   #ifdef DBASE_DEF
    // Translate for System database (RAM + ROM)
    if(Record < END_DBASE_INDEX)
    {
        Record = m_RamRecord[Record];
        m_IsItInSync[Record] = false;
    }
   #endif

    pDriver = GetDriver(Record);
    if(pDriver != nullptr)
    {
        return pDriver->Set(pData, Record, Number, SubNumber);
    }

    return SYS_NO_DRIVER;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetDriver
//
//   Parameter(s):  uint16_t                Record
//   Return:        CDataBaseInterface*     pDriver
//
//   Description:   Return the proper driver pointer and apply correction to the record value
//
//-------------------------------------------------------------------------------------------------
CDataBaseInterface* CDataBase::GetDriver(uint16_t Record)
{
    for(int i = 0; i < m_PoolCount; i++)
    {
        if(Record >= m_pDriver[i]->GetDriverIndex(RANGE_MIN))
        {
            if(Record <= m_pDriver[i]->GetDriverIndex(RANGE_MAX))
            {
                return m_pDriver[i];
            }
        }
    }
    return nullptr;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetSize
//
//   Parameter(s):  uint32_t*       pSize
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e   State
//
//   Description:   Return the size of record
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::GetSize(uint32_t* pSize, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    CDataBaseInterface* pDriver;

    pDriver = GetDriver(Record);
    if(pDriver != nullptr) return pDriver->GetSize(pSize, Record, Number, SubNumber);
    return SYS_NO_DRIVER;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: GetPointer
//
//   Parameter(s):  void**          pAddress
//                  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e   State
//
//   Description:   Get the memory pointer for the record entry if supported by the module
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::GetPointer(void** pAddress, uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    CDataBaseInterface* pDriver;

    pDriver = GetDriver(Record);
    if(pDriver != nullptr)
    {
        return pDriver->GetPointer(pAddress, Record, Number, SubNumber);
    }

    return SYS_NO_DRIVER;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: SetDB_Address
//
//   Parameter(s):  void**          pAddress
//                  uint16_t        Record    use the first record of the database for reference
//   Return:        SystemState_e   State
//
//   Description:   Set the database address if feature is supported in module
//
//-------------------------------------------------------------------------------------------------
SystemState_e CDataBase::SetDB_Address(void** pAddress, uint16_t Record)
{
    CDataBaseInterface* pDriver;

    pDriver = GetDriver(Record);
    if(pDriver != nullptr)
    {
        return pDriver->SetDB_Address(pAddress);
    }

    return SYS_NO_DRIVER;
}


//-------------------------------------------------------------------------------------------------
//
//   Function name: ForceUpdate
//
//   Parameter(s):  uint16_t        Record
//                  uint16_t        Number
//                  uint16_t        SubNumber
//   Return:        SystemState_e   State
//
//   Description:   Set the database items
//
//   Notes:         SYS_NO_DRIVER = No driver found for the record number, so it is a range check
//                                  at the same time.
//
//-------------------------------------------------------------------------------------------------
#ifdef DBASE_DEF
SystemState_e CDataBase::ForceUpdate(uint16_t Record, uint16_t Number, uint16_t SubNumber)
{
    CDataBaseInterface* pDriver;
    CDataBaseInterface* pRomDriver;
    void*               pAddress;
    uint16_t            RamRecord;
    uint16_t            RomRecord;

    // Translate for System database (RAM + ROM)
    if(Record < END_DBASE_INDEX)
    {
        RamRecord = m_RamRecord[Record];
        RomRecord = m_RomRecord[Record];
    }

    pRamDriver = GetDriver(RamRecord);
    if(pRamDriver != nullptr)
    {
        pRomDriver = GetDriver(RomRecord);
        if(pRomDriver != nullptr)
        {
        m_IsItInSync[Record] = false;

            pRamDriver->GetPointer(&pAddress, Record, Number, SubNumber);
            return pRomDriver->Set(pAddress, Record, Number, SubNumber);
        }
    }

    return SYS_NO_DRIVER;
}
#endif


//-------------------------------------------------------------------------------------------------
//
//   Function name: TimerCallBack
//
//   Parameter(s):  None
//   Return:        None
//
//
//-------------------------------------------------------------------------------------------------
void CDataBase::TimerCallBack(void)
{
}

//-------------------------------------------------------------------------------------------------


