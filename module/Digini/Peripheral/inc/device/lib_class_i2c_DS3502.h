//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_i2c_DS3502.h
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
// device definition(s)
//-------------------------------------------------------------------------------------------------

#ifdef CFG_DEVICE_DEFINITION

#define DS3502_DEFINITION X_I2C_DEVICE(I2C_DS3502, MUTEX, 0x28, sizeof(uint8_t), 100000)

#else // CFG_DEVICE_DEFINITION

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include "lib_class_i2c.h"

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class DS3502
{
    public:

        SystemState_e   Initialize                      (void* pArg);
        void            Reset                           (void);
        void            SetWiperIV                      (uint8_t InitialValue);
        void            SetWiper                        (uint8_t WiperValue);
        void            Up                                (void);
        void            Down                            (void);
        void            SetMaxValue                     (uint8_t MaxValue);

    private:

        I2C*            m_pI2C;
        uint8_t         m_WiperPos;
        uint8_t         m_WiperIV;
        uint8_t         m_MaxValue;
};

//-------------------------------------------------------------------------------------------------
// Global variable(s) and constant(s)
//-------------------------------------------------------------------------------------------------

extern class   DS3502                      POT_DS3502;

#ifdef LIB_DS3502_GLOBAL
 class   DS3502                            POT_DS3502;
#endif

//-------------------------------------------------------------------------------------------------

#endif // CFG_DEVICE_DEFINITION

