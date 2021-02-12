//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_i2c.cpp
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

#include "digini_cfg.h"
#ifdef DIGINI_USE_I2C
#include <stdint.h>
#define I2C_DRIVER_GLOBAL
#include "lib_class_STM32F7_i2c.h"
#undef  I2C_DRIVER_GLOBAL
#include "lib_io.h"
#include "lib_macro.h"
#include "bsp.h"
#include "assert.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define I2C_TIME_OUT                100                         // 100 Milliseconds

//-------------------------------------------------------------------------------------------------
//
//   Class: I2C_Driver
//
//   Description:   Class to handle I2C
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   I2C_Driver
//
//   Parameter(s):  I2C_PortInfo_t*  pPort
//
//   Description:   Initialize the I2Cx peripheral according to the specified Parameters
//
//   Note(s):        To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency
//                    (I2C peripheral input clock) must be a multiple of 10 MHz.
//
//                    I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
//                    I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains
//                  the configuration information for the specified I2C peripheral.
//
//-------------------------------------------------------------------------------------------------
I2C_Driver::I2C_Driver(const I2C_PortInfo_t* pPort)
{
    m_IsItInitialize = false;
    m_pPort          = const_cast<I2C_PortInfo_t*>(pPort);
    m_Device         = -1;
    m_Status         = SYS_DEVICE_NOT_PRESENT;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       Initialize
//
//  Parameter(s):   None
//  Return:         none
//
//  Description:    It start the initialize process from configuration in struct
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void I2C_Driver::Initialize(void)
{
    nOS_Error    Error;
    uint32_t     PriorityGroup;

    if(m_IsItInitialize == false)
    {
        m_IsItInitialize = true;
        Error = nOS_MutexCreate(&m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);
        VAR_UNUSED(Error);
    }

    m_Timeout = 0;

    NVIC_DisableIRQ(m_pPort->EV_IRQn);
    NVIC_DisableIRQ(m_pPort->ER_IRQn);

    // ---- GPIO configuration ----
    RCC->APB1RSTR |=  m_pPort->RCC_APB1_En;             // Reset I2C
    RCC->APB1RSTR &= ~m_pPort->RCC_APB1_En;             // Release reset signal of I2C
    RCC->APB1ENR  |=  m_pPort->RCC_APB1_En;

    IO_PinInit(m_pPort->SCL);
    IO_PinInit(m_pPort->SDA);

    // Configure I2C module Frequency
    m_pPort->pI2Cx->TIMINGR = m_pPort->Timing;

    PriorityGroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(m_pPort->EV_IRQn, NVIC_EncodePriority(PriorityGroup, 5, 0));
    NVIC_EnableIRQ(m_pPort->EV_IRQn);
    NVIC_SetPriority(m_pPort->ER_IRQn, NVIC_EncodePriority(PriorityGroup, 5, 0));
    NVIC_EnableIRQ(m_pPort->ER_IRQn);
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      GetStatus
//
//   Parameter(s):  None
//   Return Value:  SystemState_e
//
//   Description:   Return general status of the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::GetStatus(void)
{
    return m_Status;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           LockToDevice
//
//  Parameter(s):   uint8_t*        pDevice
//  Return:         SystemState_e   Status
//
//  Description:    This routine will configure the I2C port to work with a specific device and
//                  lock it, so any other access to the port will be block until unlock
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::LockToDevice(uint8_t Device)
{
    SystemState_e State = SYS_NOT_LOCK_TO_DEVICE;
    nOS_StatusReg   sr;

    nOS_EnterCritical(sr);

    if(m_Device == -1)
    {
        m_Device = Device;
        this->Lock();
        State = SYS_READY;
    }
    nOS_LeaveCritical(sr);

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           UnlockFromDevice
//
//  Parameter(s):   I2C_DeviceInfo_t*    pDevice
//  Return:         I2C_eStatus         Status
//
//  Description:    This routine will unlock I2C port from a specific device
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//                  if lock and no write at all if not lock to a device
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::UnlockFromDevice(uint8_t Device)
{
    SystemState_e State = SYS_WRONG_DEVICE;
    nOS_StatusReg   sr;

    nOS_EnterCritical(sr);

    if(Device == m_Device)
    {
        this->Unlock();
        m_Device = -1;
        State = SYS_READY;
    }
    else if(m_Device == -1)
    {
        State = SYS_NOT_LOCK_TO_DEVICE;
    }
    nOS_LeaveCritical(sr);

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   void*           pTxBuffer
//                  size_t          TxSize
//                  void*           pRxBuffer
//                  size_t          RxSize
//  Return:         SystemState_e   Status
//
//  Description:    Read or writes data to I2C device.
//
//  Note(s):        This is the overloaded version for multiple operation on the device
//                  Use must call LockToDevice() prior of using this function.
//                  Call to UnlockFromDevice() after multiple operation are done
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::Transfer(uint32_t Address, uint32_t AddressSize, const void* pTxBuffer, size_t TxSize, const void* pRxBuffer, size_t RxSize)
{
    uint32_t        RegisterCR1;
    uint32_t        RegisterCR2;
    nOS_TickCounter TimeOut;
    I2C_TypeDef*    pI2C;

    if(m_Device != -1)
    {
        pI2C = m_pPort->pI2Cx;

        m_Status = SYS_BUSY;
        pI2C->CR1 = 0;                        // Stop I2C module

        // Setup I2C transfer record
        m_Address     = Address;
        m_AddressSize = AddressSize;
        m_TxSize      = (pTxBuffer != nullptr) ? TxSize : 0;   // If TX buffer is null, this make sure size is 0
        m_RxSize      = (pRxBuffer != nullptr) ? RxSize : 0;   // If RX buffer is null, this make sure size is 0
        m_pTxBuffer   = (uint8_t*)pTxBuffer;
        m_pRxBuffer   = (uint8_t*)pRxBuffer;

        if((m_TxSize != 0) || (m_AddressSize != 0))
        {
            RegisterCR1 = I2C_CR1_PE     |
                          I2C_CR1_TXIE   |
                          I2C_CR1_TCIE   |
                          I2C_CR1_STOPIE;

            RegisterCR2 = ((m_TxSize + m_AddressSize) << 16)  | // Size of transfer in NBYTE
                          ((uint32_t)m_Device << 1);            // Device slave address

            if(m_RxSize == 0)                                   // if we have data to receive then IRQ on reload
            {
                RegisterCR2 |= I2C_CR2_AUTOEND;                 // STOP condition is automatically sent when NBYTES data are transferred.
            }
            else
            {
                RegisterCR1 |= I2C_CR1_RXIE;
            }

            pI2C->CR1  = RegisterCR1;
            pI2C->CR2  = RegisterCR2;
        }
        else // RX Only
        {
            pI2C->CR1 = I2C_CR1_PE    |
                        I2C_CR1_RXIE  |
                        I2C_CR1_STOPIE;
            pI2C->CR2 = I2C_CR2_AUTOEND   |
                        I2C_CR2_RD_WRN    |
                        (m_RxSize << 16)  |
                        ((uint32_t)m_Device << 1);    // Device slave address
        }

        pI2C->CR2 |= I2C_CR2_START;

        // Wait here until I2C transaction is completed or time out
        TimeOut = GetTick();
        do
        {
            nOS_Yield();
            if(TickHasTimeOut(TimeOut, I2C_TIME_OUT) == true)
            {
                // We need to reset module if I2C is jammed or in error
                this->Initialize();
            }
        }
        while(((pI2C->ISR & I2C_ISR_TC) != 1) && (m_Status == SYS_BUSY));

        return m_Status;
    }

    // If device is used by another task, we return busy, without affecting real flag for task owner
    return SYS_BUSY;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   void*           pTxBuffer
//                  size_t          TxSize
//                  void*           pRxBuffer
//                  size_t          RxSize
//                  uint8_t         Device
//  Return:         SystemState_e   Status
//
//  Description:    Read or writes data to I2C device.
//
//  Note(s):        This is the overloaded version for single operation on the device
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::Transfer(uint32_t Address, uint32_t AddressSize, const void* pTxBuffer, size_t TxSize, const void* pRxBuffer, size_t RxSize, uint8_t Device)
{
    SystemState_e State;

    if((State = this->LockToDevice(Device)) == SYS_READY)
    {
        State = this->Transfer(Address, AddressSize, pTxBuffer, TxSize, pRxBuffer, RxSize);
        this->UnlockFromDevice(Device);
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ReadRegister
//
//  Parameter(s):   uint8_t         Register
//                  void*           pRxBuffer
//                  size_t          RxSize
//                  uint8_t         Device
//
//  Return:         SystemState_e   Status
//
//  Description:    Read data from a register on the device
//
//  Note(s):        This is the overloaded version for single operation on the device
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::ReadRegister(uint8_t Register, const void* pRxBuffer, size_t RxSize, uint8_t Device)
{
    SystemState_e State;

    State = this->Transfer(uint32_t(Register), 1, (void*)nullptr, 0, pRxBuffer, RxSize, Device);

    return State;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           ReadRegister
//
//  Parameter(s):   uint8_t         Register
//                  void*           pRxBuffer
//                  size_t          RxSize
//
//  Return:         SystemState_e   Status
//
//  Description:    Read data from a register on the device
//
//  Note(s):        This is the overloaded version for multiple operation on the device
//                  Use must call LockToDevice() prior of using this function.
//                  Call to UnlockFromDevice() after multiple operation are done
//
//-------------------------------------------------------------------------------------------------
SystemState_e I2C_Driver::ReadRegister(uint8_t Register, const void* pRxBuffer, size_t RxSize)
{
    SystemState_e State;

    State = this->Transfer(Register, 1, (void*)nullptr, 0, pRxBuffer, RxSize);
    return State;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void I2C_Driver::Lock(void)
{
    while(nOS_MutexLock(&m_Mutex, NOS_WAIT_INFINITE) != NOS_OK){};
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Unlock
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Unlock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void I2C_Driver::Unlock(void)
{
    nOS_MutexUnlock(&m_Mutex);
}

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    EV_IRQHandler
//
//  Description:    This function handles I2Cx Event interrupt request.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void I2C_Driver::EV_IRQHandler()
{
    uint32_t     Status;
    I2C_TypeDef* pI2C;

    pI2C = m_pPort->pI2Cx;

    Status = pI2C->ISR;                                            // Get I2C Status

    if(Status & I2C_ISR_ARLO)                                       // --- Master Lost Arbitration ---
    {
        m_Status  = SYS_ARBITRATION_LOST;                                // Set transfer status as Arbitration Lost
        pI2C->ICR = I2C_ICR_ARLOCF;                                      // Clear Status Flags
    }
    else if(Status & I2C_ISR_BERR)                                  // --- Master Start Stop Error ---
    {
        m_Status  = SYS_BUS_ERROR;                                       // Set transfer status as Bus Error
        pI2C->ICR = I2C_ICR_BERRCF;                                      // Clear Status Flags
    }

    if(pI2C->CR1 & I2C_CR1_TXIE)
    {
        if(Status & I2C_ISR_TXE)                                    // ... Master Transmit available ...
        {
            if(m_AddressSize != 0)
            {
                pI2C->TXDR = uint8_t(m_Address >> ((m_AddressSize - 1) << 3));
                m_AddressSize--;
            }
            else if(m_TxSize == 0)
            {
                pI2C->CR1  &= ~I2C_CR1_TXIE;
                if(m_RxSize != 0)
                {
                    pI2C->CR2 &= ~I2C_CR2_NBYTES;                       // Flush the NBYTES
                    pI2C->CR2 |= (I2C_CR2_START |
                                  I2C_CR2_RD_WRN    |
                                  (m_RxSize << 16));                    // Request a read
                }
                else
                {
                    pI2C->CR2 |= I2C_CR2_STOP;
                }
            }
            else
            {
                pI2C->TXDR = *m_pTxBuffer;                                // If TX data available transmit data and continue
                m_pTxBuffer++;
                m_TxSize--;
            }
        }
    }

    if(Status & I2C_ISR_TC)
    {
        pI2C->CR2 |= I2C_CR2_AUTOEND;
    }

    if(pI2C->CR1 & I2C_CR1_RXIE)
    {
        if(Status & I2C_ISR_RXNE)                                   // ... Master Receive data available ...
        {
            *m_pRxBuffer++ = pI2C->RXDR;
        }
    }

    if(Status & I2C_ISR_STOPF)                                      // ... STOP is sent
    {
        m_Status   = SYS_READY;
        pI2C->CR1 &= ~(uint32_t)I2C_CR1_STOPIE;
        pI2C->ICR  =  I2C_ICR_STOPCF;
    }

}

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    ER_IRQHandler
//
//  Description:    This function is called in I2C_EV_IRQHandler
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void I2C_Driver::ER_IRQHandler()
{
    //this->GetLastEvent();
    //m_pPort->pI2Cx->SR1    = 0;                                                    // After a  NACK, transfer is done
    m_Status            = SYS_READY;                                                // We're done!
    m_Timeout           = 0;
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_I2C
