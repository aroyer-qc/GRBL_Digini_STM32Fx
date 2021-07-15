//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usbd.h
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
