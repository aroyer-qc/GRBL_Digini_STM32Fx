//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_usbh_interface.h
//
//*************************************************************************************************

#pragma once

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "lib_class_usbh.h"
#include "usb.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------------------------------

class USB_HostClassInterface
{
    public:

        virtual SystemState_e     InterfaceInitialize        (USBH_Handle_t* pHost)            = 0;
        virtual SystemState_e   InterfaceDeInitialize    (void)                            = 0;
        virtual SystemState_e   ClassRequest            (void)                            = 0;
        virtual SystemState_e   Process                    (void)                            = 0;
        virtual SystemState_e   SOFProcess                (void)                            = 0;
        virtual uint8_t            GetClassCode             (void)                          = 0;
    //    virtual void*            GetDataPointer             (void)                          = 0;
};

class USB_HostInterface
{
    public:

        virtual void              Initialize                 (class USB_Application* pUSB)            = 0;
        virtual SystemState_e   RegisterClass              (class USB_HostClassInterface* pClass)    = 0;
        virtual void            Start                      (void)                                  = 0;
        virtual void             Stop                       (void)                                  = 0;
        virtual void              GetStatus                 (void)                                  = 0;
        virtual void              ReadData                   (void)                                  = 0;
        virtual void              WriteData                   (void)                                  = 0;
};

//-------------------------------------------------------------------------------------------------
