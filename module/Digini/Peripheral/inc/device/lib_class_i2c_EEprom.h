//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_I2C_EEprom.h
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
#include "eeprom_cfg.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// typedef Type(s)
//-------------------------------------------------------------------------------------------------

struct EEPROM_Info_t
{
    I2C_Driver* pI2C;
    uint32_t    DeviceSize;
    uint32_t    DeviceNbOfPage;
    uint8_t     DeviceAddress;
};

//-------------------------------------------------------------------------------------------------
// Class definition(s)
//-------------------------------------------------------------------------------------------------

class EEPROM_Driver
{
    public:

                                    EEPROM_Driver           (const EEPROM_Info_t* pInfo);

        SystemState_e               Read                    (uint32_t Address, void* pDest,      size_t Size);
        SystemState_e               Write                   (uint32_t Address, const void* pSrc, size_t Size);
        //bool                        IsOperational           ();

    private:

        const EEPROM_Info_t*        m_pInfo;
        uint32_t                    m_PageMask;
        uint32_t                    m_PageSize;
        uint8_t                     m_DeviceAddressingSize;
};

//-------------------------------------------------------------------------------------------------
// constant data
//-------------------------------------------------------------------------------------------------

#include "eeprom_var.h"         // Project variable

//-------------------------------------------------------------------------------------------------

#else // USE_I2C_DRIVER == DEF_ENABLED

#pragma message("DIGINI driver for I2C must be enable and configure to use this device driver")

#endif // USE_I2C_DRIVER == DEF_ENABLED
