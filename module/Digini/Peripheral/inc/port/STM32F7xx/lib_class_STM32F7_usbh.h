//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usbh.h
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
#include <stdint.h>
#include "stm32f7xx.h"
#include "lib_typedef.h"
#include "nOS.h"
#include "lib_class_usbh_def.h"
#include "lib_class_usbh_interface.h"

//#include "usbh_def.h"
//#include "usbh_def.h"
//#include "diskio.h"
//#include "usbh_core.h"
//#include "lib_class_gpio.h"
//#include "usbh_usr.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define USBH_STACK_SIZE                             2048
#define USBH_PRIO                                5

#define USBH_ADDRESS_DEFAULT                     0
#define USBH_ADDRESS_ASSIGNED                    1
#define USBH_MPS_DEFAULT                         0x40

// Flag definition for event
#define USBH_PORT_EVENT                          0x01
#define USBH_URB_EVENT                             0x02
#define USBH_CONTROL_EVENT                         0x04
#define USBH_CLASS_EVENT                         0x08
#define USBH_STATE_CHANGED_EVENT                 0x10

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class USB_Host //: public USB_HostInterface
{
    public:
                                //    USB_Host                   ();
                                // ~USB_Host                ();

        void                        Initialize              (class USB_Application* pUSB);
        SystemState_e               RegisterClass            (class USB_HostClassInterface* pClass);
        void                         Start                     (void);
        void                         Stop                    (void);
        void                         Run                     (void);

        SystemState_e                 SelectInterface            (uint8_t interface);
        uint8_t                     FindInterface            (uint8_t Class, uint8_t SubClass, uint8_t Protocol);
        //uint8_t                      FindInterfaceIndex        (uint8_t interface_number, uint8_t alt_settings);
        uint16_t                    GetFreePipe             (void);
        uint8_t                     AllocPipe                  (uint8_t ep_addr);
        void                         FreePipe                  (uint8_t idx);
        SystemState_e                 ControlRequest            (uint8_t* pBuffer, uint16_t length);
        SystemState_e               ClrFeature                (uint8_t ep_num);

    private:

        SystemState_e                 HandleEnum                (void);
        void                          DeInitStateMachine        (void);

        SystemState_e                 SetConfig                (uint16_t cfg_idx);
        SystemState_e                 HandleControl            (void);
        SystemState_e                  GetStringDescriptor        (uint8_t string_index, uint8_t *buff, uint16_t length);
        SystemState_e                 GetDescriptor            (uint8_t req_type, uint16_t value_idx, uint8_t* buff, uint16_t length);
        USBH_DescHeader_t*             GetNextDescriptor        (uint8_t* pbuf, uint16_t* ptr);
        SystemState_e                 GetDeviceDescriptor        (uint8_t length);
        void                         ParseStringDescriptor   (uint8_t* psrc, uint8_t* pdest, uint16_t length);
        void                         ParseDeviceDescriptor   (USBH_DeviceDescriptor_t* dev_desc, uint8_t *buf, uint16_t length);
        SystemState_e                 SetAddress                (uint8_t DeviceAddress);
        SystemState_e                 GetConfigDescriptor        (uint16_t length);
        void                        ParseConfigDescriptor   (USBH_ConfigDescriptor_t* cfg_desc, uint8_t *buf, uint16_t length);
        void                         ParseInterfaceDescriptor(USBH_InterfaceDescriptor_t* if_descriptor, uint8_t *buf);
        void                         ParseEndPointDescriptor (USBH_EndPointDescriptor_t* ep_descriptor, uint8_t *buf);


        nOS_Thread              m_Handle;
        nOS_Stack               m_Stack[USBH_STACK_SIZE];
        nOS_Flag                m_Flag;
        USBH_Handle_t             m_Host;
};

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

extern "C" void USB_HostWrapper(void* pvParameters);

//-------------------------------------------------------------------------------------------------
