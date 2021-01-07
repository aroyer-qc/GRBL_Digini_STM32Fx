//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_bkpreg_dbase.h
//
//*************************************************************************************************

#ifndef __STM32F4_BKPREG_DBASE_H__
#define __STM32F4_BKPREG_DBASE_H__

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_RTC

#include "lib_typedef.h"
#include "database_cfg.h"
#include "lib_class_database.h"
//#include "lib_class_STM32F4_rtc.h"

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

#endif //  DIGINI_USE_RTC

#endif // __STM32F4_BKPREG_DBASE_H__
