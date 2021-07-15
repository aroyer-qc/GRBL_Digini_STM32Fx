//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_usbh_interface.h
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
