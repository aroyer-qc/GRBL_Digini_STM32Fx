//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_i2c.h
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

#include "stm32f7xx.h"
#include "nOS.h"
#include "lib_define.h"
#include "lib_typedef.h"
#include "lib_io.h"
#include "i2c_cfg.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

struct I2C_PortInfo_t
{
    I2C_TypeDef*        pI2Cx;
    IO_ID_e             SCL;
    IO_ID_e             SDA;
    uint32_t            RCC_APB1_En;
    uint32_t            Timing;
    uint8_t             PreempPrio;
    IRQn_Type           EV_IRQn;
    IRQn_Type           ER_IRQn;
};

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class I2C_Driver
{
    public:

                        I2C_Driver          (const I2C_PortInfo_t* pPort);

        SystemState_e   LockToDevice        (uint8_t Device);       // Set I2C to this device and lock
        SystemState_e   UnlockFromDevice    (uint8_t Device);       // Unlock I2C from device
        SystemState_e   GetStatus           (void);

        SystemState_e   ReadRegister        (uint8_t Register, const void* pRxBuffer, size_t RxSize);
        SystemState_e   ReadRegister        (uint8_t Register, const void* pRxBuffer, size_t RxSize, uint8_t Device);
        SystemState_e   Transfer            (uint32_t Address, uint32_t AddressSize, const void* pTxBuffer, size_t TxSize, const void* pRxBuffer, size_t RxSize);
        SystemState_e   Transfer            (uint32_t Address, uint32_t AddressSize, const void* pTxBuffer, size_t TxSize, const void* pRxBuffer, size_t RxSize, uint8_t Device);

        void            Initialize          (void);
        void            ER_IRQHandler       (void);
        void            EV_IRQHandler       (void);

    private:

        void            ClearBus            (void);
        uint32_t        CalculateBitMask    (uint8_t Mask, uint16_t BitConfig);
        void            Lock                (void);
        void            Unlock              (void);

        I2C_PortInfo_t*                     m_pPort;
        nOS_Mutex                           m_Mutex;
        int16_t                             m_Device;

        volatile bool                       m_IsItInitialize;
        volatile uint32_t                   m_Address;
        volatile size_t                     m_AddressSize;
        volatile size_t                     m_TxSize;
        volatile size_t                     m_RxSize;
        volatile uint8_t*                   m_pTxBuffer;
        volatile uint8_t*                   m_pRxBuffer;

        volatile uint8_t*                   m_pAddressInDevice;
        volatile uint8_t*                   m_pDataAddress;
        volatile size_t                     m_Size;
        volatile size_t                     m_AddressLengthCount;

        volatile SystemState_e              m_Status;
        volatile uint8_t                    m_Timeout;
};

//-------------------------------------------------------------------------------------------------
// constant data
//-------------------------------------------------------------------------------------------------

#include "i2c_var.h"

//-------------------------------------------------------------------------------------------------

#endif // USE_I2C_DRIVER == DEF_ENABLED
