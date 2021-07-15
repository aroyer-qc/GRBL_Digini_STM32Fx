//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_FT5336.h
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

#include "lib_class_i2c.h"
#include "lib_class_pointing_device_interface.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

// Number of event supported by this device
#define PDI_NUMBER_OF_EVENT             5

//With FT5336 : maximum 5 touches detected simultaneously
#define TS_MAX_NB_TOUCH                 ((uint32_t) FT5336_MAX_DETECTABLE_TOUCH)

#define TS_NO_IRQ_PENDING               ((uint8_t) 0)
#define TS_IRQ_PENDING                  ((uint8_t) 1)

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class FT5336 : public PointingDeviceInterface
{
    public:

        SystemState_e   Initialize                      (void* pArg);
        void            Reset                           (void);
        uint8_t         DetectEvent                     (void);
        void            GetXY                           (Cartesian_t* pCartesian);
        SystemState_e   EnableIT                        (void);
        SystemState_e   DisableIT                       (void);
        SystemState_e   ITStatus                        (void);
        SystemState_e   ClearIT                         (void);
      #ifdef DIGINI_USE_PDI_MULTI_EVENT
        ServiceEvent_e  GetGestureID                    (void);
        void            GetEventInfo                    (uint32_t EventIdx, uint32_t* pWeight, uint32_t* pArea, uint32_t* pEvent);
      #endif

    private:

        uint16_t        ReadID                          (void);
        uint32_t        Configure                       (void);

        I2C_Driver*         m_pI2C;
        uint16_t            m_IntX;
        uint16_t            m_IntY;
        bool                m_Rotate;
        uint8_t             m_CurrentActiveEventNumber; // Field holding the current number of simultaneous active touches
        uint8_t             m_CurrentActiveEventIndex;  // Field holding the touch index currently managed
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

extern class   FT5336                      PDI_FT5336;
extern class   PointingDeviceInterface*    PDI_pDriver;

#ifdef LIB_FT5336_GLOBAL
 class   FT5336                            PDI_FT5336;
 class   PointingDeviceInterface*          PDI_pDriver = &PDI_FT5336;
#endif

//-------------------------------------------------------------------------------------------------

#else // USE_I2C_DRIVER == DEF_ENABLED

#pragma message("DIGINI driver for I2C must be enable and configure to use this device driver")

#endif // USE_I2C_DRIVER == DEF_ENABLED
