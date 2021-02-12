//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_usb.cpp
//
//*************************************************************************************************

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_USE_USB
#define STM32F4_USB_GLOBAL
#include "lib_class_STM32F4_usb.h"
#undef  STM32F4_USB_GLOBAL
#include "usb_hcd_int.h"

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
CUSB::CUSB(void)
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
CUSB::~CUSB(void)
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

#endif // DIGINI_USE_USB
