//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_eeprom_dbase.h
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

#include "label_cfg.h"
#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------

#if defined(EEPROM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------

enum EEPROM_DBaseItemList_e
{
    START_EEPROM_DBASE = DBASE_INDEX_EEPROM_RANGE - 1,

  #define X_EEPROM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    EEPROM_DBASE_DEF
  #undef X_EEPROM_DBASE

    END_EEPROM_DBASE
};

struct EEpromDBaseInfo_s
{
    class CEEprom*  pEEprom;
    uint32_t        Address;
};

//-------------------------------------------------------------------------------------------------

#define NB_EEPROM_DBASE_ITEMS_CONST        ((END_EEPROM_DBASE - START_EEPROM_DBASE) - 1)

//-------------------------------------------------------------------------------------------------

class CEEPROM_DataBase : public CDataBaseInterface
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
        uint32_t        GetAddress          (uint16_t Record, uint16_t Number, uint16_t SubNumber);

        CEEprom*                m_pEEprom;
        uint32_t                m_ItemsAddress  [NB_EEPROM_DBASE_ITEMS_CONST + 1];             // need one more for boundary top limit
        static const uint16_t   m_ItemsQTY      [NB_EEPROM_DBASE_ITEMS_CONST];
        static const uint16_t   m_ItemsSubQTY   [NB_EEPROM_DBASE_ITEMS_CONST];
        static const size_t     m_ItemSize      [NB_EEPROM_DBASE_ITEMS_CONST];

};

//-------------------------------------------------------------------------------------------------

#endif // #if defined(EEPROM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
