//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_database.h
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

#include "lib_database_wrapper.h"
#include "lib_typedef.h"
#include "database_cfg.h"
#include "lib_grafx_dbase.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Global Macro
//-------------------------------------------------------------------------------------------------

#ifdef LIB_CLASS_DATABASE_GLOBAL
    #define EXTERN_LIB_CLASS_DATABASE
#else
    #define EXTERN_LIB_CLASS_DATABASE extern
#endif

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------

enum DBaseItemList_e
{
    START_DBASE_INDEX = - 1,

 #ifdef DBASE_DEF
  #define X_DBASE(ENUM_ID, RAM_RECORD, ROM_RECORD, INTERVAL, START_TIME) ENUM_ID,
    DBASE_DEF
  #undef X_DBASE
 #endif
    END_DBASE_INDEX
};

#define NB_DBASE_ITEMS_CONST        ((END_DBASE_INDEX - START_DBASE_INDEX) - 1)

//-------------------------------------------------------------------------------------------------

class CDataBaseInterface
{
    public:

                                CDataBaseInterface       ();
                               ~CDataBaseInterface       ();

        virtual SystemState_e   Initialize               (void* pConfig, size_t ObjectSize)                                          = 0;    // Initialize DataBase
        virtual SystemState_e   Get                      (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)   = 0;    // Read a Record
        virtual SystemState_e   Set                      (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber)   = 0;    // Write a Record
        virtual uint16_t        GetDriverIndex           (Range_e Range)                                                             = 0;
        virtual SystemState_e   GetSize                  (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber)   = 0;    // Get Record Size
        virtual SystemState_e   GetPointer               (void** pAddress,   uint16_t Record, uint16_t Number, uint16_t SubNumber)   = 0;    // Get Record address if it is memory location
        virtual SystemState_e   SetDB_Address            (void** pAddress)                                                           = 0;    // Set database address if supported by module
};



class CDataBase
{
    public:

                                 CDataBase               ();
                                ~CDataBase               ();
        SystemState_e            RegisterDriver          (CDataBaseInterface* DataBaseInterface);                                       // Register a hardware access database
        SystemState_e            Initialize              (void);                                                                        // Initialize DataBase
        SystemState_e            Get                     (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber = 0); // Read a Record
        SystemState_e            Set                     (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber = 0); // Write a Record
        SystemState_e            GetSize                 (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber = 0); // Get Record Size
        SystemState_e            Fill                    (const void* pData, uint16_t Record);                                          // Fill all item of a record if supported by module
        SystemState_e            GetPointer              (void** pAddress, uint16_t Record, uint16_t Number, uint16_t SubNumber = 0);   // Read a record pointer address
        SystemState_e            SetDB_Address           (void** pAddress, uint16_t Record);                                            // Set database address if supported by module
       #ifdef DBASE_DEF
        SystemState_e            ForceUpdate             (uint16_t Record, uint16_t Number, uint16_t SubNumber);
       #endif
        void                     TimerCallBack           (void);

    private:

        CDataBaseInterface*     GetDriver                (uint16_t  Record);

        uint8_t                 m_PoolCount;
        CDataBaseInterface*     m_pDriver[DBASE_INTERFACE_POOL];
        uint16_t                m_FreeIndexPointer;

       #ifdef DBASE_DEF
        bool                   m_IsItInSync [NB_DBASE_ITEMS_CONST];
        static const uint16_t  m_RamRecord  [NB_DBASE_ITEMS_CONST];
        static const uint16_t  m_RomRecord  [NB_DBASE_ITEMS_CONST];
        static const uint32_t  m_Interval   [NB_DBASE_ITEMS_CONST];
        static const uint32_t  m_StartTime  [NB_DBASE_ITEMS_CONST];
       #endif
};


//-------------------------------------------------------------------------------------------------
// Global Variable(s)
//-------------------------------------------------------------------------------------------------

EXTERN_LIB_CLASS_DATABASE class CDataBase DB_Central;

//-------------------------------------------------------------------------------------------------
// Post Include file(s)
//-------------------------------------------------------------------------------------------------

#define __DATABASE_POST_INCLUDE__
#include "database_cfg.h"
#undef __DATABASE_POST_INCLUDE__

//-------------------------------------------------------------------------------------------------
