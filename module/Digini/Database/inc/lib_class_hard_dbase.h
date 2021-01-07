//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_hard_dbase.h
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

#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------

#if defined(HARD_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------


enum HARD_DBaseItemList_e
{
    START_HARD_DBASE = DBASE_INDEX_HARD_RANGE - 1,

  #ifdef HARD_DBASE_DEF
    #define X_HARD_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE, CALLBACK) ENUM_ID,
      HARD_DBASE_DEF
    #undef X_HARD_DBASE
  #endif

    END_HARD_DBASE
};

#define NB_HARD_DBASE_ITEMS_CONST        ((END_HARD_DBASE - START_HARD_DBASE) - 1)

//----------------------------------CALLBACK---------------------------------------------------------------

class CHARD_DataBase : public CDataBaseInterface
{
    public:

        SystemState_e   Initialize          (void* pConfig, size_t ObjectSize);
        SystemState_e   Get                 (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   Set                 (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint16_t        GetDriverIndex      (Range_e Range);
        SystemState_e   GetSize             (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   GetPointer          (void** pAddress,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   SetDB_Address       (void** pAddress) {return SYS_UNSUPPORTED_FEATURE;};

    private:

        SystemState_e   CheckRange          (uint16_t Record, uint16_t Number, uint16_t SubNumber);

        static const uint16_t               m_ItemsQTY      [NB_HARD_DBASE_ITEMS_CONST];
        static const uint16_t               m_ItemsSubQTY   [NB_HARD_DBASE_ITEMS_CONST];
        static const size_t                 m_ItemSize      [NB_HARD_DBASE_ITEMS_CONST];
        static const Func_DatabaseCallBack  m_CallBack      [NB_HARD_DBASE_ITEMS_CONST];

};

//-------------------------------------------------------------------------------------------------

#endif // defined(HARD_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
