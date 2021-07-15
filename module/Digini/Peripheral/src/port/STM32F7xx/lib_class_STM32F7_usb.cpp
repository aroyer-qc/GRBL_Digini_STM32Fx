//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_usb.cpp
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

#define STM32F7_USB_GLOBAL
#include "lib_class_STM32F7_usb.h"
#undef  STM32F7_USB_GLOBAL
#include "usb_hcd_int.h"

//-------------------------------------------------------------------------------------------------

#if USE_USB_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//
//   Class: CUSB
//
//
//   Description:   Class to handle USB
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CUSB
//
//   Parameter(s):
//
//   Description:   Initializes the USB peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CUSB::CUSB()
{
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CUSB
//
//   Description:    De-initialize the USB peripheral
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
CUSB::~CUSB()
{
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
USB_MSC_HostStatus_e CUSB::GetStatus(void)
{
    if(HCD_IsDeviceConnected(&m_OTG_Core) && (m_MSC_HostStatus == USB_MSC_DEV_CONNECTED))
    {
        return m_MSC_HostStatus;
    }

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
void CUSB::Initialize(void)
{
    m_MSC_HostStatus = USB_MSC_DEV_DETACHED;
    USBH_Init(&m_OTG_Core, USB_OTG_HS_CORE_ID, &m_Host, &USBH_MSC_cb, &USR_Callbacks);
    Process();
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
USB_MSC_HostStatus_e CUSB::Process(void)
{
    if(m_MSC_HostStatus != USB_MSC_DEV_NOT_SUPPORTED)
    {
        USBH_Process(&m_OTG_Core, &m_Host);
    }

    return m_MSC_HostStatus;
}


void CUSB::IRQHandler(void)
{
    USBH_OTG_ISR_Handler(&m_OTG_Core);
}


//-------------------------------------------------------------------------------------------------

#endif // USE_USB_DRIVER == DEF_ENABLED
