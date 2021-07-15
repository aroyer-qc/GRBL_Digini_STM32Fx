//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_usb.h
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

#ifdef STM32F4_USB_GLOBAL
    #define STM32F4_USB_EXTERN
#else
    #define STM32F4_USB_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#ifdef DIGINI_FATFS_USE_USB_KEY
#include <stdint.h>
//#include "nOS.h"
//#include "lib_typedef.h"
//#include "diskio.h"
//#include "usbh_core.h"
//#include "lib_class_STM32F4_gpio.h"
#include "usbh_usr.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

// Available port on STM32F42x-3x
enum USB_State_e
{
    USB_PRESENT,
    USB_NOT_PRESENT,
};


enum USB_MSC_HostStatus_e
{
    USB_MSC_HOST_NO_INIT = 0,  // USB interface not initialized
    USB_MSC_DEV_DETACHED,      // no device connected
    USB_MSC_SPEED_ERROR,       // unsupported USB speed
    USB_MSC_DEV_NOT_SUPPORTED, // unsupported device
    USB_MSC_DEV_WRITE_PROTECT, // device is write protected
    USB_MSC_OVER_CURRENT,      // overcurrent detected
    USB_MSC_DEV_CONNECTED      // device connected and ready
};


/*
// Available port on STM32F42x-3x
enum USB_HardwarePort_e
{
    USB_OTG_FS,
    USB_OTG_HS,
    USB_OTG_HS_ULPI,
};
*/
/*
// To found how many port there is
enum USB_Port_e
{
  #define X_USB_PORT(ENUM_ID, MUTEX, PORT, PWR_CTRL, VBUS_IN, OVR_CURRENT_IN) ENUM_ID,
    USB_DEF
  #undef X_USB_PORT
    USB_NB_PORT_CONST
};

struct USB_PortInfo_t
{
    nOS_Mutex*          pMutex;
    USB_HardwarePort_e  Port;               // Port to be use
    IO_Output_e         PowerCtrl;          // Output control pin for 5V power
    IO_Input_e          VBUS_Input;         // Input pin for VBUS
    IO_Input_e          OverCurrent;        // Input pin for over current detection
};
*/
//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------
class CUSB
{
    public:
                                CUSB                       (void);
                               ~CUSB                    (void);

    void                        Initialize              (void);
    USB_MSC_HostStatus_e        GetStatus               (void);
    USB_MSC_HostStatus_e        Process                 (void);
    uint8_t                     CheckMedia              (void);
       // void                        OverCurrent_IRQHandler  ();

       void         IRQHandler (void);

        //also fatfs_usb should derive form this

        USB_OTG_CORE_HANDLE        m_OTG_Core;
        USB_MSC_HostStatus_e    m_MSC_HostStatus;  // use getter setter
        USBH_HOST                m_Host;


    private:



};


//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef STM32F4_USB_GLOBAL

CUSB USB;

#else // STM32F4_USB_GLOBAL

extern class CUSB USB;

#endif // STM32F4_USB_GLOBAL

/*

  USB_PortInfo_t USB_PortInfo[USB_NB_PORT_CONST] =
  {
    #define X_USB_PORT( ENUM_ID,  MUTEX, PORT, PWR_CTRL, VBUS_IN, OVR_CURRENT_IN )  \
                               { &MUTEX, PORT, PWR_CTRL, VBUS_IN, OVR_CURRENT_IN },
      USB_DEF
    #undef X_USB_PORT
  };
#else


  extern USB_PortInfo_t USB_PortInfo[USB_NB_PORT_CONST];

#endif


*/

#endif // DIGINI_FATFS_USE_USB_KEY
