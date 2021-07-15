//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_qspi_dbase.h
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

#if defined(QSPI_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------

enum QuadSPI_DBaseItemList_e
{
    START_QSPI_DBASE = DBASE_INDEX_QSPI_RANGE - 1,

  #ifdef QSPI_DBASE_DEF

    START_QSPI_INDEX = START_QSPI_DBASE,

   #define X_QSPI_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
     QSPI_DBASE_DEF
   #undef X_QSPI_DBASE
     END_QSPI_INDEX
  #endif

  #ifdef GFX_QSPI_DBASE_DEF

   #if defined (QSPI_DBASE_DEF)
    START_GFX_QSPI_INDEX = END_QSPI_INDEX - 1,    // Reset index if last section exist
   #else
    START_GFX_QSPI_INDEX = START_QSPI_DBASE,      // or start it at the beginning
   #endif

   #define X_GFX_QSPI_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY, ITEM_SIZE) ENUM_ID,
    GFX_QSPI_DBASE_DEF
   #undef X_GFX_QSPI_DBASE
    END_GFX_QSPI_INDEX,
  #endif

    END_QSPI_DBASE
};


struct QuasSPI_DBaseInfo_s
{
    class CQuadSPI* pQuadSPI;
    uint32_t        Address;
};

//-------------------------------------------------------------------------------------------------

#define NB_QSPI_DBASE_ITEMS_CONST        ((END_QSPI_DBASE - START_QSPI_DBASE) - 1)

//-------------------------------------------------------------------------------------------------

class CQSPI_DataBase : public CDataBaseInterface
{
    public:

        SystemState_e   Initialize          (void* pConfig, size_t ObjectSize);
        SystemState_e   Get                 (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   Set                 (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint16_t        GetDriverIndex      (Range_e Range);
        SystemState_e   GetSize             (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   GetPointer          (void** pAddress,   uint16_t Record, uint16_t Number, uint16_t SubNumber);


    private:

        SystemState_e   CheckRange          (uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint32_t        GetAddress          (uint16_t Record, uint16_t Number, uint16_t SubNumber);

        CQuadSPI*               m_pQuadSPI;
        uint32_t                m_ItemsAddress  [NB_QSPI_DBASE_ITEMS_CONST + 1];             // need one more for boundary top limit
        static const uint16_t   m_ItemsQTY      [NB_QSPI_DBASE_ITEMS_CONST];
        static const uint16_t   m_ItemsSubQTY   [NB_QSPI_DBASE_ITEMS_CONST];
        static const size_t     m_ItemSize      [NB_QSPI_DBASE_ITEMS_CONST];

};

//-------------------------------------------------------------------------------------------------

#endif // #if defined(QSPI_DBASE_DEF)

//-------------------------------------------------------------------------------------------------
