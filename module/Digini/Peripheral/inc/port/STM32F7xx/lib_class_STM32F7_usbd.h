//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usbd.h
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include <stdint.h>
#include "stm32f7xx.h"
#include "lib_typedef.h"
#include "nOS.h"
#include "lib_class_usb_interface.h"
//#include "diskio.h"
//#include "lib_class_gpio.h"

///////////// TODO

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class DeviceUSB : public USB_????_Interface
{
    public:
                            //    DeviceUSB                   ();
                            // ~DeviceUSB                ();

    void                        Initialize              (class USB_Application* pUSB);
    SystemState_e               RegisterClass            (USBH_ClassTypeDef *pclass);
    void                         Start                     (void);


    void                         Run                     (void);

    //void                          GetStatus               (void)                        {};
    //void                          ReadData                (void)                         {};
    //void                          WriteData                (void)                         {};

        nOS_Queue               m_Q_Msg;


    private:

        nOS_Thread              m_Handle;
        nOS_Stack               m_Stack[USB_OTG_STACK_SIZE];
        uint16_t                m_MsgArray[USB_NUMBER_OF_MSG];
        USBD_HandleTypeDef         m_USB_Device;
        class USB_Application*  m_pUSB;
};

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void DeviceUSB_Wrapper(void* pvParameters);

//-------------------------------------------------------------------------------------------------
