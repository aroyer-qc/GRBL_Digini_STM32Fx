//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_bkpreg_dbase.h
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

#include "lib_typedef.h"
#include "database_cfg.h"
#include "lib_class_database.h"
//#include "lib_class_STM32F7_rtc.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_RTC_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and const(s)
//-------------------------------------------------------------------------------------------------

#define BACKUP_REGISTER         0x40002850

typedef enum BKPREG_DBaseItemList_e
{
    START_BKPREG_INDEX = DBASE_INDEX_BKPREG_RANGE - 1,

  #define X_BKPREG_DBASE(ENUM_ID, ITEMS_QTY, ITEMS_SubQTY) ENUM_ID,
    BKPREG_DBASE_DEF
  #undef X_BKPREG_DBASE

    END_BKPREG_INDEX
} eBKPREG_DBaseItemList;

#define NB_BKPREG_DBASE_ITEMS_CONST        ((END_BKPREG_INDEX - START_BKPREG_INDEX) - 1)

//-------------------------------------------------------------------------------------------------

class CBKPREG_DataBase : public CDataBaseInterface
{
    public:

                        CBKPREG_DataBase    (class CRTC* pRTC);
        SystemState_e   Initialize          (void* pConfig, size_t ObjectSize);
        SystemState_e   Get                 (void*       pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   Set                 (const void* pData, uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint16_t        GetDriverIndex      (Range_e Range);
        SystemState_e   GetSize             (uint32_t* pSize,   uint16_t Record, uint16_t Number, uint16_t SubNumber);
        SystemState_e   GetPointer          (void** pAddress,   uint16_t Record, uint16_t Number, uint16_t SubNumber);

    private:

        SystemState_e   CheckRange          (uint16_t Record, uint16_t Number, uint16_t SubNumber);
        uint8_t         GetIndex            (uint16_t Record, uint16_t Number, uint16_t SubNumber);

        class CRTC*             m_pRTC;
        uint8_t                 m_ItemsIndex        [NB_BKPREG_DBASE_ITEMS_CONST];
        static const uint8_t    m_ItemsQTY          [NB_BKPREG_DBASE_ITEMS_CONST];
        static const uint8_t    m_ItemsSubQTY       [NB_BKPREG_DBASE_ITEMS_CONST];
};

//-------------------------------------------------------------------------------------------------

#endif // USE_RTC_DRIVER == DEF_ENABLED
