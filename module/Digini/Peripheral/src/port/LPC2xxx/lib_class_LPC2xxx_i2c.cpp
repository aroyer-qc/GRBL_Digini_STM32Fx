//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_LPC2xxx_i2c.cpp
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

#include <stdint.h>
#define LPC2_I2C_GLOBAL
#include "lib_class_STM32F4_i2c.h"
#undef  LPC2_I2C_GLOBAL
#include "lib_macro.h"
#include "bsp.h"
#include "assert.h"

//-------------------------------------------------------------------------------------------------

#if USE_I2C_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define CR1_CLEAR_MASK              ((uint16_t)0xFBF5)          // I2C registers Masks
#define FLAG_MASK                   ((uint32_t)0x00FFFFFF)      // I2C FLAG mask

//-------------------------------------------------------------------------------------------------
//
//   Class: CI2C
//
//
//   Description:   Class to handle I2C
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   CI2C
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Initializes the I2Cx peripheral according to the specified Parameters
//
//   Note(s):        To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency
//                    (I2C peripheral input clock) must be a multiple of 10 MHz.
//
//                    I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
//                    I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains
//                  the configuration information for the specified I2C peripheral.
//
//-------------------------------------------------------------------------------------------------
CI2C::CI2C(I2C_PortInfo_t* pPort)
{
    m_pPort   = pPort;
    m_pDevice = nullptr;
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
CI2C::~CI2C()
{
}


//-------------------------------------------------------------------------------------------------
//
//  Function:       Init
//
//  Parameter(s):   None
//  Return:         none
//
//  Description:    It start the initialize process from configuration in struct
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void CI2C::Init()
{
    uint16_t                Result;
    uint16_t                Register;
    uint16_t                FreqRange;
    uint32_t                pclk1;
    uint32_t                Temp;
    RCC_ClocksTypeDef       Clocks;
    GPIO_InitTypeDef        GPIO_InitStructure;

    m_Timeout = 0;

    this->Lock();

    // ---- GPIO configuration ----
    RCC->APB1ENR  |=  m_pPort->Clock;
    RCC->AHB1ENR  |= (m_pPort->SCL_Clock | m_pPort->SDA_Clock);
    RCC->AHB1RSTR |=  m_pPort->Clock;                                                               // Reset I2C
    RCC->AHB1RSTR &= ~m_pPort->Clock;                                                               // Release reset signal of I2C

    GPIO_PinAFConfig(m_pPort->pGPIO_SCL, m_pPort->SCL_PinSource, m_pPort->AlternateFunction);       // Connect PXx to I2C_SCL
    GPIO_PinAFConfig(m_pPort->pGPIO_SDA, m_pPort->SDA_PinSource, m_pPort->AlternateFunction);       // Connect PXx to I2C_SDA

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = m_pPort->SCL_Pin;                                                 // I2C SCL and SDA pins configuration
    GPIO_Init(m_pPort->pGPIO_SCL, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = m_pPort->SDA_Pin;
    GPIO_Init(m_pPort->pGPIO_SDA, &GPIO_InitStructure);

    // ---- Configure and enable I2C event interrupt ----
    Temp = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pPort->PreempPrio, 0x00);
    NVIC_SetPriority(m_pPort->EV_IRQn, Temp);
    NVIC_SetPriority(m_pPort->ER_IRQn, Temp);
    NVIC_EnableIRQ(m_pPort->EV_IRQn);
    NVIC_EnableIRQ(m_pPort->ER_IRQn);

    // ---- Reset peripheral and set clock ----
    RCC->APB1RSTR |=  (RCC_APB1RSTR_I2C1RST << m_pPort->HardwarePort);                              // Enable I2Cx reset state
    RCC->APB1RSTR &= ~(RCC_APB1RSTR_I2C1RST << m_pPort->HardwarePort);                              // Release I2Cx from reset state
    RCC->APB1ENR  |=  (RCC_APB1ENR_I2C1EN   << m_pPort->HardwarePort);                              // Enable I2C_PORT clock

    // ---- Peripheral software reset ----
    m_pPort->pI2Cx->CR1  =  I2C_CR1_SWRST;                                                          // Peripheral software reset
    m_pPort->pI2Cx->CR1 &= ~I2C_CR1_SWRST;

    // ---- I2C Peripheral Enable
    m_pPort->pI2Cx->CR1 |=  I2C_CR1_PE;                                                             // I2C Peripheral Enable

    // ---- I2Cx CR2 Configuration ----
    Register             = m_pPort->pI2Cx->CR2;                                                        // Get the I2Cx CR2 value
    Register            &= (uint16_t)~(I2C_CR2_FREQ);                                                  // Clear frequency FREQ[5:0] bits
    RCC_GetClocksFreq(&Clocks);                                                                        // Get pclk1 frequency value
    pclk1                = Clocks.PCLK1_Frequency;                                                    // Set frequency bits depending on pclk1 value
    FreqRange               = (uint16_t)(pclk1 / 1000000);
    Register            |= FreqRange;
    m_pPort->pI2Cx->CR2  = Register;                                                                 // Write to I2Cx CR2

    // ---- I2Cx CCR Configuration ----
    m_pPort->pI2Cx->CR1 &= (uint16_t)~(I2C_CR1_PE);                                                     // Disable the selected I2C peripheral to configure TRISE

    if(m_pPort->Speed <= 100000)                                                                       // Configure speed in standard mode
    {
        Result = (uint16_t)(pclk1 / (m_pPort->Speed << 1));                                            // Standard mode speed calculate
        Result = AbsMin(Result, 4);                                                                   // Test if CCR value is under 0x04

        Register |= Result;                                                                              // Set speed value for standard mode
        m_pPort->pI2Cx->TRISE = FreqRange + 1;                                                          // Set Maximum Rise Time for standard mode
    }
    else                                                                                             // (I2C_InitStruct->I2C_ClockSpeed <= 400000) Configure speed in fast mode
    {                                                                                                // To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral input clock) must be a multiple of 10 MHz
        Result = (uint16_t)(pclk1 / (m_pPort->Speed * 3));                                            // Fast mode speed calculate: Tlow/Thigh = 2

        if((Result & I2C_CCR_CCR) == 0)                                                                   // Test if CCR value is under 0x1
        {
            Result |= (uint16_t)0x0001;                                                              // Set minimum allowed value
        }

        Register |= (Result | I2C_CCR_FS);                                                              // Set speed value and set F/S bit for fast mode
        m_pPort->pI2Cx->TRISE = (((FreqRange * 300) / 1000) + 1);                                   // Set Maximum Rise Time for fast mode
    }

    m_pPort->pI2Cx->CCR  = Register;                                                                 // Write to I2Cx CCR
    m_pPort->pI2Cx->CR1 |= I2C_CR1_PE;                                                                // Enable the selected I2C peripheral

    // ---- I2Cx CR1 Configuration ----

    Register  = m_pPort->pI2Cx->CR1;                                                                // Get the I2Cx CR1 value
    Register &= CR1_CLEAR_MASK;                                                                        // Clear ACK, SMBTYPE and  SMBUS bits
    Register |= (I2C_Mode_I2C | I2C_Ack_Enable);                                                    // Configure I2Cx: mode and acknowledgement
                                                                                                    // Set SMBTYPE and SMBUS bits according to I2C_Mode value
                                                                                                    // Set ACK bit according to I2C_Ack value
    m_pPort->pI2Cx->CR1 = Register;                                                                    // Write to I2Cx CR1

    // ---- I2Cx OAR1 Configuration ----
    m_pPort->pI2Cx->OAR1 = I2C_AcknowledgedAddress_7bit;                                            // Set I2Cx Own Address1 and acknowledged address

    this->Unlock();
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
SystemState_e CI2C::GetStatus()
{
    return m_Status;
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
void CI2C::Lock()
{
    while(nOS_MutexLock(m_pPort->pMutex, NOS_WAIT_INFINITE) != NOS_OK){};
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
void CI2C::Unlock()
{
    nOS_MutexUnlock(m_pPort->pMutex);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           GetLastEvent
//
//  Parameter(s):   None
//  Return:         uint32_t        LastEvent
//
//  Description:    Returns the last I2Cx Event.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint32_t CI2C::GetLastEvent()
{
  uint32_t lastevent = 0;
  uint32_t flag1     = 0;
  uint32_t flag2     = 0;

  // Check the Parameters
  assert_param(IS_I2C_ALL_PERIPH(m_pPort->pI2Cx));

  // Read the I2Cx status register
  flag1 = m_pPort->pI2Cx->SR1;
  flag2 = m_pPort->pI2Cx->SR2;
  flag2 = flag2 << 16;

  // Get the last event value from I2C status register
  lastevent = (flag1 | flag2) & FLAG_MASK;

  // Return status
  return lastevent;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           LockToDevice
//
//  Parameter(s):   I2C_DeviceInfo_t*    pDevice
//  Return:         I2C_eStatus         Status
//
//  Description:    This routine will configure the I2C port to work with a specific device and
//                  lock it, so any other access to the port will be block until unlock
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//
//-------------------------------------------------------------------------------------------------
SystemState_e CI2C::LockToDevice(I2C_DeviceInfo_t* pDevice)
{
    while(nOS_MutexLock(pDevice->pMutex, NOS_WAIT_INFINITE) != NOS_OK){};

    // now we have the mutex than keep a copy for the UnlockFromDevice()
    m_pDevice       = pDevice;
    m_pPort->Speed  = pDevice->Speed;
    m_Status        = SYS_READY;
    return m_Status;
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
SystemState_e CI2C::UnlockFromDevice(I2C_DeviceInfo_t* pDevice)
{
    if(pDevice == m_pDevice)
    {
        nOS_MutexUnlock(m_pDevice->pMutex);
        m_pDevice = nullptr;
        m_Status  = SYS_DEVICE_NOT_PRESENT;
    }
    else
    {
        if(pDevice == m_pDevice)    return SYS_WRONG_DEVICE;
        else                        return SYS_NOT_LOCK_TO_DEVICE;
    }
    return m_Status;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Write
//
//  Parameter(s):   ?                   See overload
//  Return:         SystemState_e        Status
//
//  Description:    This routine writes to the I2C bus.
//
//  Note(s):        Multiple overload available
//
//-------------------------------------------------------------------------------------------------
SystemState_e CI2C::Write(uint32_t AddressInDevice, void* pDataAddress, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, AddressInDevice, pDataAddress, Size);
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint8_t Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint8_t));
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint16_t Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint16_t));
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint32_t Data)
{
    if(m_Status ==SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint32_t));
}

// Lock Version
SystemState_e CI2C::Write(uint32_t AddressInDevice, void* pDataAddress, size_t Size, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_WRITE, AddressInDevice, pDataAddress, Size);
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint8_t Data, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint8_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint16_t Data, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint16_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Write(uint32_t AddressInDevice, uint32_t Data, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_WRITE, AddressInDevice, (void*)&Data, sizeof(uint32_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Read
//
//  Parameter(s):      ?                    See overload
//                  size_t      Size        Number of byte to read
//
//  Return:         SystemState_eStatus      Error status I2C_PASS, I2C_PASS_NACK or I2C_FAIL
//
//  Description:    This routine perform read
//
//  Note(s):        Multiple overload available
//
//-------------------------------------------------------------------------------------------------
SystemState_e CI2C::Read(uint32_t AddressInDevice, void* pDataAddress, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, AddressInDevice, pDataAddress, Size);
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint8_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint8_t));
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint16_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint16_t));
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint32_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint32_t));
}

// Lock version
SystemState_e CI2C::Read(uint32_t AddressInDevice, void* pDataAddress, size_t Size, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_READ, AddressInDevice, pDataAddress, Size);
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint8_t* pData, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint8_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint16_t* pData, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint16_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}

SystemState_e CI2C::Read(uint32_t AddressInDevice, uint32_t* pData, I2C_DeviceInfo_t* pDevice)
{
    SystemState_e Status;

    this->LockToDevice(pDevice);
    Status = this->Request(ACCESS_READ, AddressInDevice, (void*)pData, sizeof(uint32_t));
    this->UnlockFromDevice(pDevice);

    return Status;
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           TickHook
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    1ms state machine timeout tick hook
//
//-------------------------------------------------------------------------------------------------
void CI2C::TickHook()
{
    if(m_Timeout > 0)
    {
        m_Timeout--;

        if(m_Timeout == 0)
        {
              m_Status = SYS_HUNG;
        }
    }
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
void CI2C::ClearBus()
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
uint32_t CI2C::CalculateBitMask(uint8_t Mask, uint16_t BitConfig)
{
    uint32_t NewMask = (uint32_t)Mask;

    while((BitConfig >>= 1) != 0)
    {
        NewMask <<= 2;
    }

    return NewMask;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Request
//
//  Parameter(s):   AccessRequest_e  Request
//                  uint32_t        AddressInDevice
//                  void*           pDataAddress
//                  size_t          Size
//  Return:         SystemState_e    Status
//
//  Description:    Read or writes data to I2C device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e CI2C::Request(AccessRequest_e Request, uint32_t AddressInDevice, void* pDataAddress, size_t Size)
{
    if(m_pDevice != nullptr)
    {
        this->Lock();

        if(m_Status == SYS_HUNG)
        {
            this->ClearBus();
        }

        // Re init interface
        this->Init();

        m_Status             = BUSY;
        m_Request             = Request;                                 // Request READ or WRITE
        m_AddressLengthCount = m_pDevice->MemPtrAddressLength;          // Device address type 8 bits or 16 bits
        m_pAddressInDevice   = (uint8_t*)&AddressInDevice;              // Register or memory address in the device
        m_pDataAddress       = (uint8_t*)pDataAddress;                  // Address in memory for data
        m_Size                 = Size;                                    // Size of the data
        m_pAddressInDevice  += (m_pDevice->MemPtrAddressLength - 1);    // Correction for endianess

        OS_EnterCritical();

        // Enable the selected I2C interrupts
        m_pPort->pI2Cx->CR2 |= (I2C_IT_EVT | I2C_IT_ERR);

        // Generate a START condition
        m_Timeout = (uint8_t)(1000 / I2C_TICK_HOOK_TIME);
        m_pPort->pI2Cx->CR1  |= I2C_CR1_START;

        OS_ExitCritical();
        while(m_Status == BUSY){};
        this->Unlock();

        if(m_Status == SYS_HUNG)
        {
            return TIME_OUT;
        }
        return SYS_READY;
    }
    return SYS_NOT_LOCK_TO_DEVICE;
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
void CI2C::EV_IRQHandler()
{
    uint32_t I2C_ISR_Ev;

    I2C_ISR_Ev = this->GetLastEvent();

    switch(I2C_ISR_Ev)
    {
        // ---- Master Mode ----
        case  I2C_EVENT_MASTER_MODE_SELECT:
        {
            if((m_Request == ACCESS_WRITE) || (m_AddressLengthCount > 0))
            {
                m_pPort->pI2Cx->DR = (m_pDevice->PhysicalAddress & ~I2C_OAR1_ADD0);     // Send slave address for write
            }
            else
            {
                m_pPort->pI2Cx->DR = (m_pDevice->PhysicalAddress | I2C_OAR1_ADD0);        // Send slave address for read
            }
            break;
        }

        // ---- Master Transmitter ----
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:                                // Test on I2Cx EV6 and first EV8 and clear them
        {
            m_pPort->pI2Cx->CR2 |= I2C_IT_BUF;                                             // Next we send data buf, enable buffer interrupt
            // Test on I2Cx EV8 and clear it
            // buffer register empty, DR shifting out

            // Pass through in write so no "break" here
        }

        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:                                          // Without BTF, EV8
        {
            if(m_AddressLengthCount > 0)                                                // If transmitting address bytes
            {
                m_pPort->pI2Cx->DR = *m_pAddressInDevice;                                  // Send memory address byte
                m_pAddressInDevice--;
                m_AddressLengthCount--;                                                      // Decrement address length
            }
            else if(m_Request == ACCESS_READ)                                            // Read request?
            {
                m_pPort->pI2Cx->CR2 &= ~I2C_IT_BUF;                                        // Done sending address before read
            }
            else if(m_Size > 0)                                                             // Bytes to be transmitted?
            {
                m_pPort->pI2Cx->DR = *m_pDataAddress++;                                    // Send next byte
                m_Size--;                                                                 // Decrement data length counter
            }
            else                                                                        // If transmitting last byte
            {
                m_pPort->pI2Cx->CR2 &= ~I2C_IT_BUF;                                        // Done sending buffer
            }
            break;
        }

        // Test on I2Cx EV8 and clear it
        // buffer register empty, DR empty
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if(m_Request == ACCESS_READ)                                                // If was transmitting address bytes before read
            {
                m_pPort->pI2Cx->CR1    |= I2C_CR1_START;                                    // Generate a START condition to read data
            }
            else                                                                        // If we are done transmitting�
            {
                m_pPort->pI2Cx->CR1    |= I2C_CR1_STOP;                                    // Generate a STOP condition
                m_pPort->pI2Cx->CR2    &= ~(I2C_IT_EVT | I2C_IT_ERR);                        // Disable the selected I2C interrupts
                m_Status = SYS_READY;
            }
            break;
        }

        // Master Receiver -----------------------------------------------------

        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:                                   // EVENT EV6
        {
            if(m_Size == 1)
            {
                m_pPort->pI2Cx->CR1 &= ~I2C_CR1_ACK;                                    // Disable the acknowledgement
                m_pPort->pI2Cx->CR1 |= I2C_CR1_STOP;                                    // Generate a STOP condition
            }
            m_pPort->pI2Cx->CR2 |= I2C_IT_BUF;                                            // Next we receive data buffer
            break;
        }

        case I2C_EVENT_MASTER_BYTE_RECEIVED:                                            // EVENT EV7
        {                                                                               // Test on I2Cx EV7 and clear it
            if(m_Size > 0)                                                                // If waiting for bytes?
            {
                *m_pDataAddress = (uint8_t)m_pPort->pI2Cx->DR;                            // Store I2Cx received data
                m_pDataAddress++;
                m_Size--;
                if(m_Size == 1)                                                            // One more byte to go?
                {
                    m_pPort->pI2Cx->CR1 &= ~I2C_CR1_ACK;                                // Disable the acknowledgement
                    m_pPort->pI2Cx->CR1 |= I2C_CR1_STOP;                                // Generate a STOP condition
                }
                else if(m_Size == 0)                                                     // last byte received?
                {
                    m_pPort->pI2Cx->CR2 &= ~(I2C_IT_EVT | I2C_IT_ERR);                    // Disable the selected I2C interrupts
                    m_Status = SYS_READY;                                                    // We're done!
                }
            }
            else                                                                        // Not waiting for bytes, not supposed to be here
            {
                m_pPort->pI2Cx->CR2 &= ~(I2C_IT_EVT | I2C_IT_ERR);                        // Disable the selected I2C interrupts
                m_Status = SYS_READY;                                                        // We're done!
            }
            break;
        }

        default:                                                                        // Not supposed to be here
        {
            // reset all flags : Do not modify configuration here
            // timeout will be generated
            I2C_ISR_Ev          = this->GetLastEvent();
            I2C_ISR_Ev          = (uint8_t)m_pPort->pI2Cx->DR;
              m_pPort->pI2Cx->SR1 = 0;
            break;
        }
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
void CI2C::ER_IRQHandler()
{
    this->GetLastEvent();
    m_pPort->pI2Cx->SR1    = 0;                                                    // After a  NACK, transfert is done
    m_Status            = SYS_READY;                                                // We're done!
    m_Timeout           = 0;
}

//-------------------------------------------------------------------------------------------------

#endif // USE_I2C_DRIVER == DEF_ENABLED
