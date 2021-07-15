//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usbd.cpp
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

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include "lib_class_STM32F7_usbd.h"
//#include "usb_hcd_int.h"
#include "stm32f7xx_ll_usb.h"

//-------------------------------------------------------------------------------------------------
//
//  Name:           DeviceUSB_Wrapper
//
//  Parameter(s):   void* pvParameters
//  Return:         void
//
//  Description:    main() for the task1
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
extern "C" void DeviceUSBD_Wrapper(void* pvParameters)
{
    (static_cast<DeviceUSB>(pvParameters))->Run();
}

//-------------------------------------------------------------------------------------------------
//
//   Class: DeviceUSB
//
//
//   Description:   Class to handle USB device
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   DeviceUSB
//
//   Parameter(s):
//
//   Description:   Initializes the USB peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
//DeviceUSB::DeviceUSB()
//{
//}

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void DeviceUSB::Initialize(class USB_Application* pUSB)
{
    m_pUSB = pUSB;

    // Initialize IO, clocks, etc...
    m_pUSB->Initialize();

    // Create USB Host Queue
    nOS_QueueCreate(&this->m_Q_Msg, &this->m_MsgArray[0], sizeof(uint16_t), USB_NUMBER_OF_MSG);

    // Create USB Host Task
    nOS_ThreadCreate(&this->m_Handle,
                     DeviceUSB_Wrapper,
                     this,
                     &this->m_Stack[0],
                    USBD_STACK_SIZE,
                    USBD_PRIO);

    // todo

}
//-------------------------------------------------------------------------------------------------
//
//   Function:        RegisterClass
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e DeviceUSB::RegisterClass(USBH_ClassTypeDef *pClass)
{
    return SYSTEM_OK;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:        Start
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void DeviceUSB::Start(void)
{
      /* Start the low level driver  */
    //__HAL_LOCK(hhcd);
    USB_OTG_FS->GAHBCFG |= USB_OTG_GAHBCFG_GINT;

    USB_DriveVbus(USB_OTG_FS, 1);
    //__HAL_UNLOCK(hhcd);


     m_pUSB->DriverVBUS(true);        // Activate VBUS on the port

}

//-------------------------------------------------------------------------------------------------
//
//  Name:           run
//
//  Parameter(s):   void
//  Return:         void
//
//  Description:    main() for the task1
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void DeviceUSB::Run(void)
{
    USBH_HandleTypeDef Event;

    for(;;)
    {
        nOS_QueueRead(&this->m_Q_Msg, &Event, NOS_WAIT_INFINITE);

        if(Event.status == osEventMessage)
        {
            USBH_Process(&m_USB_Host);
        }
    }
}

#if 0
//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
USB_MSC_HostStatus_e DeviceUSB::GetStatus(void)
{

    /*
    if(HCD_IsDeviceConnected(&m_OTG_Core) && (m_MSC_HostStatus == USB_MSC_DEV_CONNECTED))
    {
        return m_MSC_HostStatus;
    }
*/
    return USB_MSC_DEV_DETACHED;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:
//
//   Parameter(s):
//   Return Value:
//
//   Description:
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
USB_MSC_HostStatus_e DeviceUSB::Process(void)
{
    /* USB Host Background task */
    USBH_Process(&hUSBHost);



/*
    if(m_MSC_HostStatus != USB_MSC_DEV_NOT_SUPPORTED)
    {
        USBH_Process(&m_OTG_Core, &m_Host);
    }
*/
    return m_MSC_HostStatus;
}


void CUSB::IRQHandler(void)
{

}


//-------------------------------------------------------------------------------------------------
#endif
