//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_pointing_device_interface.h
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

#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class PointingDeviceInterface
{
    public:

        virtual SystemState_e   Initialize                      (void* pArg)                = 0;
        virtual void            Reset                           (void)                      = 0;
        virtual uint8_t         DetectEvent                     (void)                      = 0;
        virtual void            GetXY                           (Cartesian_t* pCartesian)   = 0;
      #ifdef GRAFX_PDI_INTERRUPT_IO
        virtual SystemState_e   EnableIT                        (void)                      = 0;
        virtual SystemState_e   DisableIT                       (void)                      = 0;
        virtual SystemState_e   ITStatus                        (void)                      = 0;
        virtual SystemState_e   ClearIT                         (void)                      = 0;
      #endif
      #ifdef DIGINI_USE_PDI_MULTI_EVENT
        virtual ServiceEvent_e  GetGestureID                    (void)                      = 0;
        virtual void            GetEventInfo                    (uint32_t  EventIdx,
                                                                 uint32_t* pWeight,
                                                                 uint32_t* pArea,
                                                                 uint32_t* pEvent)          = 0;
      #endif
};

//-------------------------------------------------------------------------------------------------

