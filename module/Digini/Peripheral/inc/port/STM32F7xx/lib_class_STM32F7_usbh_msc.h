#pragma once

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "stm32f7xx.h"
#include "lib_typedef.h"
#include "lib_class_usbh_interface.h"

//#include "usbh_core.h"
//#include "usbh_msc_bot.h"
//#include "usbh_msc_scsi.h"

#ifndef MAX_SUPPORTED_LUN
    #define MAX_SUPPORTED_LUN       2
#endif

#define USB_REQ_BOT_RESET                0xFF
#define USB_REQ_GET_MAX_LUN              0xFE

// Interface Descriptor field values for HID Boot Protocol
#define MSC_BOT                                        0x50
#define MSC_TRANSPARENT                                0x06


class USB_HostMassStorage : public USB_HostClassInterface
{
    public:
    
    
        SystemState_e             InterfaceInitialize                (USBH_Handle_t* pHost);
        SystemState_e             InterfaceDeInitialize             (void);
        SystemState_e             ClassRequest                    (void);
        SystemState_e             Process                            (void);
        SystemState_e             SOFProcess                        (void);
        //void*                    GetMSC_Handle                    (void) {return m_MSC_Handle;}
        uint8_t                    GetClassCode                    (void);

        // Common APIs
        bool                    IsReady                            (void);

        // APIs for LUN
        int8_t                     GetMaxLUN                        (void);
        bool                    UnitIsReady                     (uint8_t lun);
        SystemState_e             GetLUN_Info                        (uint8_t lun, MSC_LUN_t *info);

        void                     BOT_Initialize                    (void);
        void                     BOT_REQ_GetMaxLUN                (uint8_t *Maxlun);
        SystemState_e             BOT_Abort                        (uint8_t lun, uint8_t dir);
        SystemState_e             BOT_REQ_Reset                    (void);

        SystemState_e             SCSI_Write                        (uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length);
        SystemState_e             SCSI_Read                        (uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length);
        SystemState_e           SCSI_Inquiry                    (uint8_t lun, SCSI_StdInquiryData_t* inquiry);
        SystemState_e             SCSI_ReadCapacity                (uint8_t lun, SCSI_Capacity_t* capacity);
        SystemState_e             SCSI_TestUnitReady                (uint8_t lun);
        SystemState_e             SCSI_RequestSense                (uint8_t lun, SCSI_Sense_t* sense_data);

        SystemState_e             Read                            (uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length);
        SystemState_e             Write                            (uint8_t lun, uint32_t address, uint8_t *pbuf, uint32_t length);

        SystemState_e             RdWrProcess                        (uint8_t lun);
        SystemState_e             BOT_Process                        (uint8_t lun);
        BOT_CSW_Status_e         DecodeCSW                        (void);

        
    private:

        USBH_Handle_t*             m_pHost;
        MSC_Handle_t             m_MSC_Handle;
};
