//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_ram_dbase.h
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

//#include "digini_cfg.h"
#include "lib_class_database.h"

//-------------------------------------------------------------------------------------------------

#if defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
// enum(s) and struct(s)
//-------------------------------------------------------------------------------------------------

enum RAM_DBaseItemList_e
{
    START_RAM_DBASE = DBASE_INDEX_RAM_RANGE - 1,

  #ifdef RAM_DBASE_DEF
    START_RAM_INDEX = START_RAM_DBASE,

   #define X_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    RAM_DBASE_DEF
   #undef X_RAM_DBASE
    END_RAM_INDEX,
  #endif

  #ifdef GFX_RAM_DBASE_DEF
   #if defined (RAM_DBASE_DEF)
    START_GFX_RAM_INDEX = END_RAM_INDEX - 1,    // Reset index if last section exist
   #else
    START_GFX_RAM_INDEX = START_RAM_DBASE,      // or start it at the beginning
   #endif

   #define X_GFX_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    GFX_RAM_DBASE_DEF
   #undef X_GFX_RAM_DBASE
    END_GFX_RAM_INDEX,
  #endif

  #ifdef NV_RAM_DBASE_DEF
   #if defined (GFX_RAM_DBASE_DEF)
    START_NV_RAM_INDEX = END_GFX_RAM_INDEX - 1, // Reset index if last section exist
   #elif defined (RAM_DBASE_DEF)
    START_NV_RAM_INDEX = END_RAM_INDEX - 1,     // Reset index if first section exist
   #else
    START_NV_RAM_INDEX = START_RAM_DBASE,       // or start it at the beginning
   #endif

   #define X_NV_RAM_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    NV_RAM_DBASE_DEF
   #undef X_NV_RAM_DBASE
    END_NV_RAM_INDEX,
  #endif

    END_RAM_DBASE,
    NB_RAM_DBASE_ITEMS_CONST = (END_RAM_DBASE - START_RAM_DBASE) - 2,
};

struct RAM_DBaseRegionPointer_t
{
  #ifdef RAM_DBASE_DEF
    uint8_t* pRam;
  #endif

  #ifdef GFX_RAM_DBASE_DEF
    uint8_t* pGFX_Ram;
  #endif

  #ifdef NV_RAM_DBASE_DEF
    uint8_t* pNV_Ram;
  #endif
};

//-------------------------------------------------------------------------------------------------

class CRAM_DataBase : public CDataBaseInterface
{
    public:

        SystemState_e   Initialize          (void* pConfig, size_t ObjectSize);
        SystemState_e   Get                 (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   Set                 (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint16_t        GetDriverIndex      (Range_e Range);
        SystemState_e   GetSize             (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   GetPointer          (void** pAddress,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   SetDB_Address       (void** pAddress);
    private:

        SystemState_e   CheckRange          (uint16_t Record, uint16_t Number, uint16_t SubNumber);


        uint8_t*                m_ItemsPointer      [NB_RAM_DBASE_ITEMS_CONST + 1];             // need one more for boundary top limit
        static const uint16_t   m_ItemsQTY          [NB_RAM_DBASE_ITEMS_CONST];
        static const uint16_t   m_ItemsSubQTY       [NB_RAM_DBASE_ITEMS_CONST];
        static const size_t     m_ItemSize          [NB_RAM_DBASE_ITEMS_CONST];
};

//-------------------------------------------------------------------------------------------------

#endif // defined(RAM_DBASE_DEF) || defined(GFX_RAM_DBASE_DEF) || defined(NV_RAM_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
