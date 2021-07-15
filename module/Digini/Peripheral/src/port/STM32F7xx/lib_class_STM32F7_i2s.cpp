//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_i2s.cpp
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

// TODO TODO TODO TODO....  base this on I2C driver

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include <stdint.h>
#define STM32F7_I2S_GLOBAL
#include "lib_class_STM32F7_i2s.h"
#undef  STM32F7_I2S_GLOBAL
#include "lib_STM32F7_gpio.h"
#include "lib_macro.h"
#include "bsp.h"
#include "assert.h"

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define CR1_CLEAR_MASK              ((uint16_t)0xFBF5)          // I2C registers Masks
#define FLAG_MASK                   ((uint32_t)0x00FFFFFF)      // I2C FLAG mask
#define I2S_TIME_OUT                100                         // 100 Milliseconds

//-------------------------------------------------------------------------------------------------
// forward declaration(s)
//-------------------------------------------------------------------------------------------------

#define X_I2S(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, EVENT_IRQ_NB, ERROR_IRQ_NB, GPIO_SCL, AF_SCL, PIN_SCL, SCL_GPIO_CLOCK, GPIO_SDA, AF_SDA, PIN_SDA, SDA_GPIO_CLOCK) \
  extern "C" void I2S##I2S_NUMBER##_EV_IRQHandler();      \
  extern "C" void I2S##I2S_NUMBER##_ER_IRQHandler();
  I2S_DEF
#undef X_I2S

//-------------------------------------------------------------------------------------------------
//
//   Class: I2S
//
//   Description:   Class to handle I2S
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   I2S
//
//   Parameter(s):  I2S_PortInfo_t*  pPort
//
//   Description:   Initialize the I2Sx peripheral according to the specified Parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
I2S::I2S(I2S_PortInfo_t* pPort)
{
    m_pPort   = pPort;
    m_Device  = -1;
    m_Status  = SYS_DEVICE_NOT_PRESENT;
}


//-------------------------------------------------------------------------------------------------
//
//   Destructor:   I2S
//
//  Description:    De-initializes the I2Sx peripheral
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
I2S::~I2S()
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
void I2S::Initialize(void)
{
    nOS_Error               Error;
    uint32_t                PriorityGroup;

    // TODO put an initialization flag
    Error = nOS_MutexCreate(&this->m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);
    VAR_UNUSED(Error);

    m_Timeout = 0;

    NVIC_DisableIRQ(m_pPort->EV_IRQn);
    NVIC_DisableIRQ(m_pPort->ER_IRQn);

    // ---- GPIO configuration ----
    RCC->APB1ENR  |=  m_pPort->Clock;
    RCC->APB1RSTR |=  m_pPort->Clock;                                   // Reset I2C
    RCC->APB1RSTR &= ~m_pPort->Clock;                                   // Release reset signal of I2C

    IO_PinInit(&m_pPort->SCL);
    IO_PinInit(&m_pPort->SDA);

    // Configure I2C module Frequency
    m_pPort->pI2Cx->TIMINGR = m_pPort->Timing;

    PriorityGroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(m_pPort->EV_IRQn, NVIC_EncodePriority(PriorityGroup, 5, 0));
    NVIC_EnableIRQ(m_pPort->EV_IRQn);
    NVIC_SetPriority(m_pPort->ER_IRQn, NVIC_EncodePriority(PriorityGroup, 5, 0));
    NVIC_EnableIRQ(m_pPort->ER_IRQn);


    ///  ___________________________________________________________________

    uint16_t tmpreg = 0;
    uint16_t i2sdiv = 2;
    uint16_t i2sodd = 0;
    packetlength = 1;
    uint32_t tmp = 0;
    uint32_t i2sclk = 0;

    // Init the low level hardware : GPIO, CLOCK, CORTEX...etc
    HAL_I2S_MspInit(m_pPort->pI2Sx);

    /*----------------------- SPIx I2SCFGR & I2SPR Configuration -----------------*/
    /* Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits */
    m_pPort->pI2Sx->I2SCFGR &= ~(SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CKPOL |
                                 SPI_I2SCFGR_I2SSTD | SPI_I2SCFGR_PCMSYNC | SPI_I2SCFGR_I2SCFG |
                                 SPI_I2SCFGR_I2SE | SPI_I2SCFGR_I2SMOD);
    m_pPort->pI2Sx->I2SPR = 0x0002;

    // Get the I2SCFGR register value
    tmpreg = m_pPort->pI2Sx->I2SCFGR;

    // If the default value has to be written, reinitialize i2sdiv and i2sodd
    if(hi2s->Init.AudioFreq == I2S_AUDIOFREQ_DEFAULT)
    {
      i2sodd = (uint16_t)0;
      i2sdiv = (uint16_t)2;
    }
    /* If the requested audio frequency is not the default, compute the prescaler */
    else
    {
      /* Check the frame length (For the Prescaler computing) *******************/
      if(hi2s->Init.DataFormat == I2S_DATAFORMAT_16B)
      {
        /* Packet length is 16 bits */
        packetlength = 1;
      }
      else
      {
        /* Packet length is 32 bits */
        packetlength = 2;
      }

      /* Get I2S source Clock frequency  ****************************************/

      /* If an external I2S clock has to be used, the specific define should be set
      in the project configuration or in the stm32f3xx_conf.h file */
      if(hi2s->Init.ClockSource == I2S_CLOCK_EXTERNAL)
      {
        /* Set the I2S clock to the external clock  value */
        i2sclk = EXTERNAL_CLOCK_VALUE;
      }
      else
      {
              i2sclk = I2S_GetClockFreq(hi2s);                // Get the I2S source clock value
      }

      if(hi2s->Init.MCLKOutput == I2S_MCLKOUTPUT_ENABLE)    // Compute the Real divider depending on the MCLK output state, with a floating point
      {
        tmp = (uint16_t)(((((i2sclk / 256) * 10) / hi2s->Init.AudioFreq)) + 5);                    // MCLK output is enabled
      }
      else
      {
        tmp = (uint16_t)(((((i2sclk / (32 * packetlength)) *10 ) / hi2s->Init.AudioFreq)) + 5);    // MCLK output is disabled
      }

      tmp = tmp / 10;                                // Remove the floating point
      i2sodd = (uint16_t)(tmp & (uint16_t)0x0001);    // Check the parity of the divider
      i2sdiv = (uint16_t)((tmp - i2sodd) / 2);        // Compute the i2sdiv prescaler
      i2sodd = (uint16_t) (i2sodd << 8);            // Get the Mask for the Odd bit (SPI_I2SPR[8]) register
    }

    // Test if the divider is 1 or 0 or greater than 0xFF
    if((i2sdiv < 2) || (i2sdiv > 0xFF))
    {
      // Set the default values
      i2sdiv = 2;
      i2sodd = 0;
    }

    // Write to SPIx I2SPR register the computed value
    m_pPort->pI2Sx->I2SPR = (uint16_t)((uint16_t)i2sdiv | (uint16_t)(i2sodd | (uint16_t)hi2s->Init.MCLKOutput));

    // Configure the I2S with the I2S_InitStruct values
    tmpreg |= (uint16_t)((uint16_t)SPI_I2SCFGR_I2SMOD | (uint16_t)(hi2s->Init.Mode |
                         (uint16_t)(hi2s->Init.Standard | (uint16_t)(hi2s->Init.DataFormat |
                         (uint16_t)hi2s->Init.CPOL))));

    m_pPort->pI2Sx->I2SCFGR = tmpreg;        // Write to SPIx I2SCFGR
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
SystemState_e I2C::GetStatus(void)
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
{
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
        m_TxSize  = (pTxBuffer != nullptr) ? TxSize : 0;   // If TX buffer is null, this make sure size is 0
        m_RxSize  = (pRxBuffer != nullptr) ? RxSize : 0;   // If RX buffer is null, this make sure size is 0
        m_pTxBuffer = (uint8_t*)pTxBuffer;
        m_pRxBuffer = (uint8_t*)pRxBuffer;

        if(m_TxSize != 0)
        {
            RegisterCR1 = I2C_CR1_PE     |
                          I2C_CR1_TXIE   |
                          I2C_CR1_TCIE   |
                          I2C_CR1_STOPIE;

            RegisterCR2 = (m_TxSize << 16)  |           // Size of transfer in NBYTE
                          ((uint32_t)m_Device << 1);    // Device slave address

            if(m_RxSize == 0)                           // if we have data to receive then IRQ on reload
            {
                RegisterCR2 |= I2C_CR2_AUTOEND;         // STOP condition is automatically sent when NBYTES data are transferred.
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
/*
void I2C::ClearBus(void)
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

//-------------------------------------------------------------------------------------------------
//
//  Name:           CalculateBitMask
//
//  Parameter(s):   uint8_t     Mask
//                  uint16_t    BitConfig
//  Return:         uint32_t    NewMask
//
//  Description:    Calculate a mask according to configuration pin
//
//  Note(s):        this function serve ClearBus
//
//-------------------------------------------------------------------------------------------------
/*
uint32_t I2C::CalculateBitMask(uint8_t Mask, uint16_t BitConfig)
{
    uint32_t NewMask = (uint32_t)Mask;

    while((BitConfig >>= 1) != 0)
    {
        NewMask <<= 2;
    }

    return NewMask;
}
*/

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
            if(m_TxSize == 0)
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
void I2C::ER_IRQHandler()
{
    //this->GetLastEvent();
    //m_pPort->pI2Cx->SR1    = 0;                                                    // After a  NACK, transfer is done
    m_Status            = SYS_READY;                                                // We're done!
    m_Timeout           = 0;
}


//-------------------------------------------------------------------------------------------------
//  Handler:        I2Cx_EV_IRQHandler and I2Cx_EE_IRQHandler
//
//  Description:    Handle interrupt I2Cx Event handler
//
//  Note(s):        Wrapper for C++ for handler
//
//-------------------------------------------------------------------------------------------------
#define X_I2C(I2C_NUMBER, CLOCK, SPEED, PREEMP_PRIO, EVENT_IRQ_NB, ERROR_IRQ_NB, GPIO_SCL, AF_SCL, PIN_SCL, SCL_GPIO_CLOCK, GPIO_SDA, AF_SDA, PIN_SDA, SDA_GPIO_CLOCK) \
  extern "C" void I2C##I2C_NUMBER##_EV_IRQHandler()       \
  {                                                       \
      I2C_Port##I2C_NUMBER.EV_IRQHandler();               \
  }                                                       \
  extern "C" void I2C##I2C_NUMBER##_ER_IRQHandler()       \
  {                                                       \
      I2C_Port##I2C_NUMBER.ER_IRQHandler();               \
  }
  I2C_DEF
#undef X_I2C

//-------------------------------------------------------------------------------------------------
