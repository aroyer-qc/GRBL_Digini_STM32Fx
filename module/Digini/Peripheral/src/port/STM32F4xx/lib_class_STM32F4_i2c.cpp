//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_i2c.cpp
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
#define STM32F4_I2C_GLOBAL
#include "lib_class_STM32F4_i2c.h"
#undef  STM32F4_I2C_GLOBAL
#include "lib_macro.h"
#include "bsp.h"
#include "assert.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define CR1_CLEAR_MASK                              ((uint16_t)0xFBF5)          // I2C registers Masks
#define FLAG_MASK                                   ((uint32_t)0x00FFFFFF)      // I2C FLAG mask
#define I2C_TIME_OUT                                100                         // 100 Milliseconds

#define I2C_EVENT_MASTER_MODE_SELECT                ((uint32_t)0x00030001)      // BUSY, MSL and SB flag
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ((uint32_t)0x00070082)      // BUSY, MSL, ADDR, TXE and TRA flags
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED     ((uint32_t)0x00030002)      // BUSY, MSL and ADDR flags


#define I2C_EVENT_MASTER_BYTE_RECEIVED              ((uint32_t)0x00030040)      // BUSY, MSL and RXNE flags
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING          ((uint32_t)0x00070080)      // TRA, BUSY, MSL, TXE flags
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED           ((uint32_t)0x00070084)      // TRA, BUSY, MSL, TXE and BTF flags

//-------------------------------------------------------------------------------------------------
//
//   Class: CI2C
//
//
//   Description:   Class to handle I2C
//
//-------------------------------------------------------------------------------------------------

should be base on STM32F7!!

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   I2C
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the I2Cx peripheral according to the specified Parameters
//
//   Note(s):       To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency
//                  (I2C peripheral input clock) must be a multiple of 10 MHz.
//
//                  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
//                  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains
//                  the configuration information for the specified I2C peripheral.
//
//-------------------------------------------------------------------------------------------------
I2C::I2C(I2C_PortInfo_t* pPort)
{
    m_pPort   = pPort;
    m_Device  = -1;
    m_Status  = SYS_DEVICE_NOT_PRESENT;
}

//-------------------------------------------------------------------------------------------------
//
//   Destructor:   CI2C
//
//   Parameter(s):
//   Return Value:
//
//  Description:    Deinitializes the I2Cx peripheral
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
I2C::~I2C()
{
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
void I2C::Initialize()
{
#if 0
    nOS_Error Error;
    uint32_t                Temp;

    // mettre un flag d'initialisation
    Error = nOS_MutexCreate(&this->m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);
    VAR_UNUSED(Error);

    m_Timeout = 0;

    NVIC_DisableIRQ(m_pPort->EV_IRQn);
    NVIC_DisableIRQ(m_pPort->ER_IRQn);

    // ---- GPIO configuration ----
    RCC->APB1ENR  |=  m_pPort->Clock;
    RCC->AHB1RSTR |=  m_pPort->Clock;                                                               // Reset I2C
    RCC->AHB1RSTR &= ~m_pPort->Clock;                                                               // Release reset signal of I2C

    IO_PinInit(&m_pPort->SCL);
    IO_PinInit(&m_pPort->SDA);

    // ---- Configure and enable I2C event interrupt ----
    Temp = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pPort->PreempPrio, 0x00);
    NVIC_SetPriority(m_pPort->EV_IRQn, Temp);
    NVIC_EnableIRQ(m_pPort->EV_IRQn);
    NVIC_SetPriority(m_pPort->ER_IRQn, Temp);
    NVIC_EnableIRQ(m_pPort->ER_IRQn);

    // ---- Reset peripheral and set clock ----
    RCC->APB1ENR  |=  (RCC_APB1ENR_I2C1EN   << m_pPort->HardwarePort);                              // Enable I2C_PORT clock
    RCC->APB1RSTR |=  (RCC_APB1RSTR_I2C1RST << m_pPort->HardwarePort);                              // Enable I2Cx reset state
    RCC->APB1RSTR &= ~(RCC_APB1RSTR_I2C1RST << m_pPort->HardwarePort);                              // Release I2Cx from reset state

    // ---- Peripheral software reset ----
    m_pPort->pI2Cx->CR1  =  I2C_CR1_SWRST;                                                          // Peripheral software reset
    m_pPort->pI2Cx->CR1 &= ~I2C_CR1_SWRST;

    // ---- I2C Peripheral Enable
    m_pPort->pI2Cx->CR1 |=  I2C_CR1_PE;                                                             // I2C Peripheral Enable

    // ---- I2Cx CR2 Configuration ----
    Register             = m_pPort->pI2Cx->CR2;                                                     // Get the I2Cx CR2 value
    Register            &= (uint16_t)~(I2C_CR2_FREQ);                                               // Clear frequency FREQ[5:0] bits
    RCC_GetClocksFreq(&Clocks);                                                                     // Get pclk1 frequency value
    pclk1                = Clocks.PCLK1_Frequency;                                                  // Set frequency bits depending on pclk1 value
    FreqRange            = (uint16_t)(pclk1 / 1000000);
    Register            |= FreqRange;
    m_pPort->pI2Cx->CR2  = Register;                                                                // Write to I2Cx CR2

/*
    // ---- I2Cx CCR Configuration ----
    m_pPort->pI2Cx->CR1 &= (uint16_t)~(I2C_CR1_PE);                                                 // Disable the selected I2C peripheral to configure TRISE
    if(m_pPort->Speed <= 100000)                                                                    // Configure speed in standard mode
    {
        Result = (uint16_t)(pclk1 / (m_pPort->Speed << 1));                                         // Standard mode speed calculate
        Result = AbsMin(Result, 4);                                                                 // Test if CCR value is under 0x04

        Register |= Result;                                                                         // Set speed value for standard mode
        m_pPort->pI2Cx->TRISE = FreqRange + 1;                                                      // Set Maximum Rise Time for standard mode
    }
    else                                                                                            // (I2C_InitStruct->I2C_ClockSpeed <= 400000) Configure speed in fast mode
    {                                                                                               // To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral input clock) must be a multiple of 10 MHz
        Result = (uint16_t)(pclk1 / (m_pPort->Speed * 3));                                          // Fast mode speed calculate: Tlow/Thigh = 2

        if((Result & I2C_CCR_CCR) == 0)                                                             // Test if CCR value is under 0x1
        {
            Result |= (uint16_t)0x0001;                                                             // Set minimum allowed value
        }

        Register |= (Result | I2C_CCR_FS);                                                          // Set speed value and set F/S bit for fast mode
        m_pPort->pI2Cx->TRISE = (((FreqRange * 300) / 1000) + 1);                                   // Set Maximum Rise Time for fast mode
    }

    m_pPort->pI2Cx->CCR  = Register;                                                                // Write to I2Cx CCR
    m_pPort->pI2Cx->CR1 |= I2C_CR1_PE;                                                              // Enable the selected I2C peripheral
*/

    // ---- I2Cx CR1 Configuration ----

    Register  = m_pPort->pI2Cx->CR1;                                                                // Get the I2Cx CR1 value
    Register &= CR1_CLEAR_MASK;                                                                     // Clear ACK, SMBTYPE and  SMBUS bits
    Register |= (I2C_Mode_I2C | I2C_Ack_Enable);                                                    // Configure I2Cx: mode and acknowledgement
                                                                                                    // Set SMBTYPE and SMBUS bits according to I2C_Mode value
                                                                                                    // Set ACK bit according to I2C_Ack value
    m_pPort->pI2Cx->CR1 = Register;                                                                 // Write to I2Cx CR1

    // ---- I2Cx OAR1 Configuration ----
    m_pPort->pI2Cx->OAR1 = I2C_AcknowledgedAddress_7bit;                                            // Set I2Cx Own Address1 and acknowledged address
#endif
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
SystemState_e I2C::GetStatus()
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
SystemState_e I2C::LockToDevice(uint8_t Device)
    if(m_Device == -1)
    {
        m_Device = Device;
        this->Lock();
        return SYS_READY;
    }
    return SYS_NOT_LOCK_TO_DEVICE;
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
SystemState_e I2C::UnlockFromDevice(uint8_t Device)
{
    if(Device == m_Device)
    {
        this->Unlock();
        m_Device = -1;
        return SYS_READY;
    }

    if(m_Device == -1)
    {
        return SYS_NOT_LOCK_TO_DEVICE;
    }

    return SYS_WRONG_DEVICE;
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
void I2C::Lock(void)
{
    while(nOS_MutexLock(&this->m_Mutex, NOS_WAIT_INFINITE) != NOS_OK){};
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
void I2C::Unlock(void)
{
    nOS_MutexUnlock(&this->m_Mutex);
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
SystemState_e I2C::Transfer(void* pTxBuffer, size_t TxSize, void* pRxBuffer, size_t RxSize)
{
    nOS_TickCounter TimeOut;
    I2C_TypeDef*    pI2C;

    if(m_Device != -1)
    {
        pI2C = m_pPort->pI2Cx;

        m_Status = SYS_BUSY;
        pI2C->CR1 = 0;                        // Stop I2C module

        // Setup I2C transfer record
        m_TxSize  = (pTxBuffer != nullptr) ? TxSize : 0;   // If TX buffer is null, this make sure size is 0
        m_RxSize  = (pRxBuffer != nullptr) ? RxSize : 0;   // If RX buffer is null, this make sure size is 0
        m_pTxBuffer = (uint8_t*)pTxBuffer;
        m_pRxBuffer = (uint8_t*)pRxBuffer;

        // Enable the selected I2C interrupts
        pI2C->CR2 |= (I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);

        // Generate a START condition
        pI2C->CR1  |= I2C_CR1_START;

        // Wait here until I2C transaction is completed or time out
        TimeOut = GET_START_TIME();
        do
        {
            nOS_Yield();
            if(HAS_TIMED_OUT(TimeOut, I2C_TIME_OUT) == true)
            {
                // We need to reset module if I2C is jammed or in error
                this->Initialize();
            }
        }
        while(/*((pI2C->ISR & I2C_ISR_TC) != 1) &&*/ (m_Status == SYS_BUSY));

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
SystemState_e I2C::Transfer(void* pTxBuffer, size_t TxSize, void* pRxBuffer, size_t RxSize, uint8_t Device)
{
    SystemState_e State;

    if((State = this->LockToDevice(Device)) == SYS_READY)
    {
        State = this->Transfer(pTxBuffer, TxSize, pRxBuffer, RxSize);
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
SystemState_e I2C::ReadRegister(uint8_t Register, void* pRxBuffer, size_t RxSize, uint8_t Device)
{
    SystemState_e State;

    State = this->Transfer(&Register, sizeof(uint8_t), pRxBuffer, RxSize, Device);

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
SystemState_e I2C::ReadRegister(uint8_t Register, void* pRxBuffer, size_t RxSize)
{
    SystemState_e State;

    State = this->Transfer(&Register, sizeof(uint8_t), pRxBuffer, RxSize);
    return State;
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
void I2C::EV_IRQHandler()
{
    uint32_t     Status;
    I2C_TypeDef* pI2C;

    pI2C = m_pPort->pI2Cx;

    Status = ((uint32_t)pI2C->SR1 | ((uint32_t)pI2C->SR2 << 16)) & FLAG_MASK;

    switch(Status)
    {
        // ---- Master Mode ----
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            if(m_TxSize != 0)
            {
                pI2C->DR = (m_Device /*<< */ & ~I2C_OAR1_ADD0);                         // Send slave address for write
            }
            else
            {
                pI2C->DR = (m_Device /*<< */ | I2C_OAR1_ADD0);                          // Send slave address for read
            }
        }
        break;

        // ---- Master Transmitter ----
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:                                // Test on I2Cx EV6 and first EV8 and clear them
        {
            pI2C->CR2 |= I2C_CR2_ITBUFEN;                                               // Next we send data buf, enable buffer interrupt
            // Test on I2Cx EV8 and clear it
            // buffer register empty, DR shifting out

        }
    // Pass through in write so no "break" here

        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:                                        // Without BTF, EV8
        {
            if(m_TxSize != 0)
            {
                pI2C->DR = *m_pTxBuffer;                                                // If TX data available transmit data and continue
                m_pTxBuffer++;
                m_TxSize--;
            }
            else if(m_RxSize != 0)
            {
                pI2C->CR2 &= ~I2C_CR2_ITBUFEN;                                          // Done sending data before reading data
            }
            else                                                                        // If transmitting last data
            {
                pI2C->CR2 &= ~I2C_CR2_ITBUFEN;                                          // Done sending buffer
            }
        }
        break;

        // Test on I2Cx EV8 and clear it
        // buffer register empty, DR empty
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if(m_RxSize != 0)                                                           // If was transmitting address bytes before read
            {
                pI2C->CR1 |= I2C_CR1_START;                                             // Generate a START condition to read data
                // maybe need a stop to make sure it's a restart condition see Logic probe to confirm this
            }
            else                                                                        // If we are done transmitting\A0
            {
                pI2C->CR1   |= I2C_CR1_STOP;                                            // Generate a STOP condition
                pI2C->CR2   &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);                    // Disable the selected I2C interrupts
                m_Status = SYS_READY;
            }
        }
        break;

        // Master Receiver -----------------------------------------------------

        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:                                   // EVENT EV6
        {
            if(m_RxSize == 1)
            {
                pI2C->CR1 &= ~I2C_CR1_ACK;                                              // Disable the acknowledgement
                pI2C->CR1 |= I2C_CR1_STOP;                                              // Generate a STOP condition
            }
            pI2C->CR2 |= I2C_CR2_ITBUFEN;                                               // Next we receive data buffer
        }
        break;

        case I2C_EVENT_MASTER_BYTE_RECEIVED:                                            // EVENT EV7
        {                                                                               // Test on I2Cx EV7 and clear it
            if(m_RxSize > 0)                                                            // If waiting for bytes?
            {
                *m_pRxBuffer = (uint8_t)pI2C->DR;                                       // Store I2Cx received data
                m_pRxBuffer++;
                m_RxSize--;

                if(m_RxSize == 1)                                                       // One more byte to go?
                {
                    pI2C->CR1 &= ~I2C_CR1_ACK;                                          // Disable the acknowledgement
                    pI2C->CR1 |= I2C_CR1_STOP;                                          // Generate a STOP condition
                }
                else if(m_RxSize == 0)                                                  // last byte received?
                {
                    pI2C->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);                  // Disable the selected I2C interrupts
                    m_Status = SYS_READY;                                               // We're done!
                }
            }
            else                                                                        // Not waiting for bytes, not supposed to be here
            {
                pI2C->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);                      // Disable the selected I2C interrupts
                m_Status = SYS_READY;                                                   // We're done!
            }
        }
        break;

        default:                                                                        // Not supposed to be here
        {
            // reset all flags : Do not modify configuration here
            // timeout will be generated
            Status = pI2C->SR1 | pI2C->SR2 | pI2C->DR;                                  // Dummy read
            pI2C->SR1 = 0;
        }
        break;
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
void I2C::ER_IRQHandler()
{
    uint32_t     Status;
    I2C_TypeDef* pI2C;

    pI2C = m_pPort->pI2Cx;

    Status    = pI2C->SR1 | pI2C->SR2 | pI2C->DR;       // dummy read
    VAR_UNUSED(Status);
    pI2C->SR1 = 0;                                      // After a  NACK, transfert is done
    m_Status  = SYS_READY;                              // We're done!
    m_Timeout = 0;
}

//-------------------------------------------------------------------------------------------------

#endif // DIGINI_USE_I2C





/*

//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearBus
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    Send clock sequence on the bus until SDA get low at ~ 100 KHz
//
//  Note(s):        Will get out a after a few try
//
//-------------------------------------------------------------------------------------------------
void I2C::ClearBus()
{
    uint8_t Count = 0;

    m_pPort->pGPIO_SCL->OTYPER |= m_pPort->SCL_Pin;                                                 // Set I2C SCL as output open-drain
    m_pPort->pGPIO_SDA->OTYPER |= m_pPort->SDA_Pin;                                                 // Set I2C SDA as output open-drain
    m_pPort->pGPIO_SCL->MODER  &= ~CalculateBitMask(0x03, m_pPort->SCL_Pin);                        // Set I2C SCL as output open-drain
    m_pPort->pGPIO_SDA->MODER  &= ~CalculateBitMask(0x03, m_pPort->SDA_Pin);                        // Set I2C SDA as output open-drain
    m_pPort->pGPIO_SCL->MODER  |=  CalculateBitMask(0x01, m_pPort->SCL_Pin);
    m_pPort->pGPIO_SDA->MODER  |=  CalculateBitMask(0x01, m_pPort->SDA_Pin);
    m_pPort->pGPIO_SCL->BSRRL   = m_pPort->SCL_Pin;                                                 // Reset SCL
    m_pPort->pGPIO_SDA->BSRRH   = m_pPort->SDA_Pin;                                                 // Assert SDA

    if((m_pPort->pGPIO_SDA->IDR & m_pPort->SDA_Pin) == 0)                                           // If SDA hung
    {
        do                                                                                          // loop while SDA hung
        {
            m_pPort->pGPIO_SCL->BSRRL = m_pPort->SCL_Pin;
            BSP_Delay_uSec(5);
            m_pPort->pGPIO_SCL->BSRRH = m_pPort->SCL_Pin;
            BSP_Delay_uSec(5);
            Count++;
        }
        while(((m_pPort->pGPIO_SDA->IDR & m_pPort->SDA_Pin) == 0) && (Count < 20));

        m_pPort->pGPIO_SCL->BSRRL = m_pPort->SCL_Pin;                                               // Generate stop condition
        m_pPort->pGPIO_SDA->BSRRL = m_pPort->SDA_Pin;
        m_pPort->pGPIO_SCL->BSRRH = m_pPort->SCL_Pin;
        m_pPort->pGPIO_SDA->BSRRH = m_pPort->SDA_Pin;
    }

    m_pPort->pGPIO_SCL->MODER  &= ~CalculateBitMask(0x03, m_pPort->SCL_Pin);                        // Set I2C SCL as alternate function open-drain
    m_pPort->pGPIO_SDA->MODER  &= ~CalculateBitMask(0x03, m_pPort->SDA_Pin);                        // Set I2C SDA as alternate function open-drain
    m_pPort->pGPIO_SCL->MODER  |=  CalculateBitMask(0x02, m_pPort->SCL_Pin);
    m_pPort->pGPIO_SDA->MODER  |=  CalculateBitMask(0x02, m_pPort->SDA_Pin);
}


*/

