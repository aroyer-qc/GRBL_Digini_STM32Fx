//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_rom_dbase.h
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

//#include "project_def.h"
#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------

#if defined(ROM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------

enum ROM_DBaseItemList_e
{
    START_ROM_DBASE = DBASE_INDEX_ROM_RANGE - 1,

  #define X_ROM_DBASE(ENUM_ID, ITEM_ADDRESS, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    ROM_DBASE_DEF
  #undef X_ROM_DBASE

    END_ROM_DBASE
};

#define NB_ROM_DBASE_ITEMS_CONST        ((END_ROM_DBASE - START_ROM_DBASE) - 1)

//-------------------------------------------------------------------------------------------------

class CROM_DataBase : public CDataBaseInterface
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

        SystemState_e    CheckRange          (uint16_t Record, uint16_t Number, uint16_t SubNumber);

        static const uint8_t*   m_ItemsPointer  [NB_ROM_DBASE_ITEMS_CONST];
        static const uint16_t   m_ItemsQTY      [NB_ROM_DBASE_ITEMS_CONST];
        static const uint16_t   m_ItemsSubQTY   [NB_ROM_DBASE_ITEMS_CONST];
        static const size_t     m_ItemSize      [NB_ROM_DBASE_ITEMS_CONST];

};

//-------------------------------------------------------------------------------------------------

#endif // #if defined(ROM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
