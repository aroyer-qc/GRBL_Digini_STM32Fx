//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_spi.cpp
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

#define STM32F7_SPI_GLOBAL
#include "lib_class_STM32F7_spi.h"
#undef STM32F7_SPI_GLOBAL
#include "lib_utility.h"

//-------------------------------------------------------------------------------------------------

#if USE_SPI_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//
//   Class: SPI_Driver
//
//
//   Description:   Class to handle SPI
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   SPI_Driver
//
//   Parameter(s):
//
//   Description:   Initializes the SPIx peripheral according to the specified Parameters
//
//   Note(s):
//
//                  SPIx: where x can be 1 to 6 to select the SPI peripheral.
//                  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that contains
//                  the configuration information for the specified SPI peripheral.
//
//-------------------------------------------------------------------------------------------------
SPI_Driver::SPI_Driver(SPI_PortInfo_t* pPort)
{
    m_pPort   = pPort;
    m_pDevice = nullptr;
    m_Status  = SYS_DEVICE_NOT_PRESENT;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      GetStatus
//
//   Parameter(s):
//   Return Value:
//
//   Description:    SystemState_e  Return general status of the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::GetStatus(void)
{
    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      Initialize
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Initialize this SPI port
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::Initialize(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    m_pDevice       = nullptr;
    m_Status        = SYS_DEVICE_NOT_PRESENT;
    m_pCallBackTick = nullptr;

    // ---- Configure and enable SPI interrupt ----
    if(m_pPort->CallBackISR != nullptr)
    {
        uint32_t Priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pPort->PreempPrio, 0x00);
        NVIC_SetPriority(m_pPort->IRQn, Priority);
        NVIC_EnableIRQ(m_pPort->IRQn);
    }

    // ---- GPIO configuration ----
    RCC->AHB1ENR  |= (m_pPort->SCLK_Clock | m_pPort->MISO_Clock | m_pPort->MOSI_Clock);             // Activate Clock on port for these pin

    GPIO_PinAFConfig(m_pPort->pGPIO_SCLK, m_pPort->SCLK_PinSource, m_pPort->AlternateFunction);     // Connect PXx to SCLK
    GPIO_PinAFConfig(m_pPort->pGPIO_MISO, m_pPort->MISO_PinSource, m_pPort->AlternateFunction);     // Connect PXx to MISO
    GPIO_PinAFConfig(m_pPort->pGPIO_MOSI, m_pPort->MOSI_PinSource, m_pPort->AlternateFunction);     // Connect PXx to MOSI

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                                                   // Global pin setup for SPI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = m_pPort->SCLK_Pin;                                                // SPI SCLK, MISO, MOSI pins configuration
    GPIO_Init(m_pPort->pGPIO_SCLK, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = m_pPort->MISO_Pin;
    GPIO_Init(m_pPort->pGPIO_MISO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = m_pPort->MOSI_Pin;
    GPIO_Init(m_pPort->pGPIO_MOSI, &GPIO_InitStructure);

    // if chip select controlled by module, this part need to be tested and debugged!!
 /*  if((m_pPort->portConfig & SPI_CR1_SSM_ENABLE) == SPI_CR1_SSM_ENABLE)                            // Configure CS if function is enable
    {
        RCC->AHB1ENR |= m_pPort->CS_Clock;

        GPIO_PinAFConfig(m_pPort->pGPIO_CS, m_pPort->CS_PinSource, m_pPort->AlternateFunction);     // Connect PXx to CS

        GPIO_InitStructure.GPIO_Pin = m_pPort->CS_Pin;
        GPIO_Init(m_pPort->pGPIO_CS, &GPIO_InitStructure);
    }
*/
    switch(m_pPort->HardwarePort)
    {
        case SPI1_HARD_PORT:
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB2RSTR |=  RCC_APB2RSTR_SPI1RST;                                                    // Enable SPI1 reset state
            RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;                                                 // Release SPI1 from reset state
            RCC->APB2ENR  |=  RCC_APB2ENR_SPI1EN;                                                    // Enable SPI_PORT clock
            RCC->AHB1ENR  |=  RCC_AHB1ENR_DMA2EN;
            break;
        }

        case SPI2_HARD_PORT:
        case SPI3_HARD_PORT:
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB1RSTR |=  (RCC_APB1RSTR_SPI2RST << (m_pPort->HardwarePort - SPI2_HARD_PORT));    // Enable SPI2 or SPI3 reset state
            RCC->APB1RSTR &= ~(RCC_APB1RSTR_SPI2RST << (m_pPort->HardwarePort - SPI2_HARD_PORT));   // Release SPI2 or SPI3 from reset state
            RCC->APB1ENR  |=  (RCC_APB1ENR_SPI2EN   << (m_pPort->HardwarePort - SPI2_HARD_PORT));    // Enable SPI_PORT clock
            RCC->AHB1ENR  |=  RCC_AHB1ENR_DMA1EN;
            break;
        }

        case SPI4_HARD_PORT:
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB2RSTR |=  RCC_APB2RSTR_SPI4RST;                                                    // Enable SPI4 reset state
            RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI4RST;                                                 // Release SPI4 from reset state
            RCC->APB2ENR  |=  RCC_APB2ENR_SPI4EN;                                                    // Enable SPI_PORT clock
            RCC->AHB1ENR  |=  RCC_AHB1ENR_DMA2EN;
            break;
        }
        case SPI5_HARD_PORT:
        case SPI6_HARD_PORT:
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB2RSTR |=  (RCC_APB2RSTR_SPI5RST << (m_pPort->HardwarePort - SPI5_HARD_PORT));    // Enable SPI5 or SPI6 reset state
            RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI5RST << (m_pPort->HardwarePort - SPI5_HARD_PORT));   // Release SPI6 or SPI6 from reset state
            RCC->APB2ENR  |=  (RCC_APB2ENR_SPI5EN   << (m_pPort->HardwarePort - SPI5_HARD_PORT));    // Enable SPI_PORT clock
            RCC->AHB1ENR  |=   RCC_AHB1ENR_DMA2EN;
            break;
        }
    }

    m_pPort->pSPIx->CR1      = m_pPort->portConfig;                                                 // Configuration for SPIx Port
    m_pPort->pSPIx->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;                                                 // Activate the SPIx mode (Reset I2SMOD bit in I2SCFGR register)

    nOS_MutexCreate(&m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LockToDevice
//
//  Parameter(s):   SPI_DeviceInfo_t*    pDevice
//  Return:         SystemState_e        Status
//
//  Description:    This routine will configure the SPI port to work with a specific device and
//                  lock it, so any other access to the port will be block until unlock
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::LockToDevice(SPI_DeviceInfo_t* pDevice)
{
    if(m_pDevice == nullptr)
    {
        while(nOS_MutexLock(&m_Mutex, NOS_WAIT_INFINITE) != NOS_OK){};

        m_pCallBackTick = nullptr;
        m_pDevice       = pDevice;
        m_Status        = SYS_READY;
        m_SlowSpeed     = GetPrescalerFromSpeed(m_pDevice->SlowSpeed);
        m_FastSpeed     = GetPrescalerFromSpeed(m_pDevice->FastSpeed);
        this->Config(LOW_SPEED);
    }

    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           UnlockFromDevice
//
//  Parameter(s):   SPI_DeviceInfo_t*    pDevice
//  Return:         SystemState_e        Status
//
//  Description:    This routine will unlock SPI port from a specific device
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//                  if lock and no write at all if not lock to a device
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::UnlockFromDevice(SPI_DeviceInfo_t* pDevice)
{
    m_pCallBackTick = nullptr;

    if(pDevice == m_pDevice)
    {
        nOS_MutexUnlock(&m_Mutex);
        m_pDevice = nullptr;
        m_Status  = SYS_DEVICE_NOT_PRESENT;
    }
    else
    {
        if(pDevice != m_pDevice) return SYS_WRONG_DEVICE;
        else                     return SYS_NOT_LOCK_TO_DEVICE;
    }
    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Read
//
//  Parameter(s):   ?                   See overload
//                  size_t  Size        Number of byte to read
//
//  Return:         SystemState_e        Error status SPI_PASS, SPI_PASS_NACK or SPI_FAIL
//
//  Description:    This routine perform read.
//
//  Note(s):        Multiple overload available
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::Read(uint8_t* pBuffer, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, pBuffer, Size);
}

SystemState_e SPI_Driver::Read(uint8_t* pData)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_READ, (uint8_t*)pData, sizeof(uint8_t));
}

SystemState_e SPI_Driver::Read(uint16_t* pData)
{
    SystemState_e State;

    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    State = this->Request(ACCESS_READ,  (uint8_t*)pData, sizeof(uint16_t));
    LIB_uint16_t_Swap(pData);
    return State;
}

SystemState_e SPI_Driver::Read(uint32_t* pData)
{
    SystemState_e State;

    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    State = this->Request(ACCESS_READ, (uint8_t*)pData, sizeof(uint32_t));
    LIB_uint32_t_Swap(pData);
    return State;
}

// Lock version
SystemState_e SPI_Driver::Read(uint8_t* pBuffer, size_t Size, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_READ, pBuffer, Size);
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Read(uint8_t* pData, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_READ, (uint8_t*)pData, sizeof(uint8_t));
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Read(uint16_t* pData, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_READ, (uint8_t*)pData, sizeof(uint16_t));
    LIB_uint16_t_Swap(pData);
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Read(uint32_t* pData, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_READ, (uint8_t*)pData, sizeof(uint32_t));
    LIB_uint32_t_Swap(pData);
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Write
//
//  Parameter(s):   ?                   See overload
//  Return:         SystemState_e        Status
//
//  Description:    This routine writes to the SPI bus.
//
//  Note(s):        Multiple overload available
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::Write(const uint8_t* pBuffer, size_t Size)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, (uint8_t*)pBuffer, Size);
}

SystemState_e SPI_Driver::Write(uint8_t Data)
{
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint8_t));
}

SystemState_e SPI_Driver::Write(uint16_t Data)
{
    LIB_uint16_t_Swap(&Data);
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint16_t));
}

SystemState_e SPI_Driver::Write(uint32_t Data)
{
    LIB_uint32_t_Swap(&Data);
    if(m_Status == SYS_DEVICE_NOT_PRESENT) return SYS_DEVICE_NOT_PRESENT;
    return this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint32_t));
}

// Lock Version
SystemState_e SPI_Driver::Write(const uint8_t* pBuffer, size_t Size, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_WRITE, (uint8_t*)pBuffer, Size);
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Write(uint8_t Data, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint8_t));
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Write(uint16_t Data, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    LIB_uint16_t_Swap(&Data);
    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint16_t));
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

SystemState_e SPI_Driver::Write(uint32_t Data, SPI_DeviceInfo_t* pDevice)
{
    SystemState_e PriorityState;
    SystemState_e State;

    LIB_uint32_t_Swap(&Data);
    if((State     = this->LockToDevice(pDevice)) != SYS_READY) return State;
    PriorityState = this->Request(ACCESS_WRITE, (uint8_t*)&Data, sizeof(uint32_t));
    State         = this->UnlockFromDevice(pDevice);
    if(PriorityState != SYS_READY) State = PriorityState;

    return State;
}

//-------------------------------------------------------------------------------------------------
//
//  Function:       GetPrescalerFromSpeed
//
//  Parameter(s):   None
//  Return:         none
//
//  Description:    Calculate the prescaler value for requested speed
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint16_t SPI_Driver::GetPrescalerFromSpeed(uint32_t Speed)
{
    int i;
    uint16_t Prescaler;

    for(i = 0; i < 8; i++)
    {
        Prescaler = ((i + 1) << 1);

        if((APB2_CLK / Speed) < Prescaler)
        {
            return Prescaler;
        }
    }
    return i;     // 'i' is at an invalid value
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      Lock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Lock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::Lock(void)
{
    while(nOS_MutexLock(&m_Mutex, NOS_WAIT_INFINITE) != NOS_OK) {};
}


//-------------------------------------------------------------------------------------------------
//
//   Function:      Unlock
//
//   Parameter(s):
//   Return Value:
//
//   Description:   Unlock the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::Unlock(void)
{
    nOS_MutexUnlock(&m_Mutex);
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Config
//
//  Parameter(s):   DeviceSpeed_e  Speed
//  Return:         None
//
//  Description:    Set speed on the SPI port
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::Config(uint32_t Speed)
{
    m_pPort->pSPIx->CR1 &= ~SPI_CR1_SPE;                                                            // Disable SPIx
    m_pPort->pSPIx->CR1 &= ~SPI_PRESCALER_MASK;                                                     // Clear prescaler
//i have remove enum... so create it locally
//    m_pPort->pSPIx->CR1 |= (Speed == FAST_SPEED) ? m_FastSpeed : m_SlowSpeed;                       // Apply prescaler
    m_pPort->pSPIx->CR1 |= SPI_CR1_SPE;                                                             // Enable SPIx
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           RegisterDriverTick
//
//  Parameter(s):   void(*CallBackTick)()           Function pointer on Tickhook for device driver
//  Return:         none
//
//  Description:    1ms state machine timeout tick hook
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::RegisterDriverTick(void (*CallBackTick)())
{
    m_pCallBackTick = CallBackTick;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           TickHook
//
//  Parameter(s):   none
//  Return:         none
//
//  Description:    1ms state machine timeout tick hook (must be added to OS Tick hook function)
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::TickHook(void)
{
    if(m_pCallBackTick != nullptr)
    {
        this->m_pCallBackTick();
    }

    if(m_Timeout > 0)
    {
        m_Timeout--;

        if(m_Timeout == 0) m_Status = SYS_HUNG;
        else               m_Status = SYS_BUSY;
    }
}


//-------------------------------------------------------------------------------------------------
//
//  Name:           Request
//
//  Parameter(s):   AccessRequest_e  Request
//                  void*           pBuffer
//                  size_t          Size
//  Return:         SPI_eStatus     Status
//
//  Description:    Read or writes data to SPI device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::Request(AccessRequest_e Request, uint8_t* pBuffer, size_t Size)
{
    if(m_pDevice != nullptr)
    {
      #if SPI_USE_DMA_TRANSFERT == 1

        uint8_t    DummyTX = 0xFF;
        bool    bTransmitCompleted;

        this->Lock();

        DMA_DeInit(m_pPort->pDMA_RX);
        m_pPort->pDMA_RX->NDTR = Size;
        m_pPort->pDMA_RX->PAR  = (uint32_t)&(m_pPort->pSPIx->DR);

        DMA_DeInit(m_pPort->pDMA_TX);
        m_pPort->pDMA_TX->NDTR = Size;
        m_pPort->pDMA_TX->PAR  = (uint32_t)&(m_pPort->pSPIx->DR);

        m_pPort->pDMA_RX->CR = m_pPort->IRQ_Source        | m_pPort->DMA_ChannelRX |
                               DMA_DIR_PeripheralToMemory | DMA_Mode_Normal        | DMA_FIFOMode_Disable        | DMA_Priority_VeryHigh    | DMA_PeripheralInc_Disable |
                               DMA_PeripheralBurst_Single | DMA_MemoryBurst_Single | DMA_PeripheralDataSize_Byte | DMA_MemoryDataSize_Byte;

        m_pPort->pDMA_TX->CR = m_pPort->IRQ_Source        | m_pPort->DMA_ChannelTX |
                               DMA_DIR_MemoryToPeripheral | DMA_Mode_Normal        | DMA_FIFOMode_Disable        | DMA_Priority_VeryHigh    | DMA_PeripheralInc_Disable |
                               DMA_PeripheralBurst_Single | DMA_MemoryBurst_Single | DMA_PeripheralDataSize_Byte | DMA_MemoryDataSize_Byte;

        switch(Request)
        {
            case ACCESS_READ:
            {
                // DMA Channel configuration SPIx RX
                m_pPort->pDMA_RX->CR   |= DMA_MemoryInc_Enable;
                m_pPort->pDMA_RX->M0AR  = (uint32_t)pBuffer;

                // DMA Channel configuration SPIx TX
                m_pPort->pDMA_TX->CR   |= DMA_MemoryInc_Disable;
                m_pPort->pDMA_TX->M0AR  = (uint32_t)&DummyTX;
                break;
            }

            case ACCESS_WRITE:
            {
                // DMA Channel configuration SPIx RX
                m_pPort->pDMA_RX->CR   |= DMA_MemoryInc_Disable;
                m_pPort->pDMA_RX->M0AR  = (uint32_t)&DummyTX;
                // DMA Channel configuration SPIx TX
                m_pPort->pDMA_TX->CR   |= DMA_MemoryInc_Enable;
                m_pPort->pDMA_TX->M0AR  = (uint32_t)pBuffer;
                break;
            }
        }
        //dummy read to clear data register
        void(m_pPort->pSPIx->DR);

        // Enable RX & TX Channel
        m_pPort->pDMA_RX->CR |= DMA_SxCR_EN;
        m_pPort->pDMA_TX->CR |= DMA_SxCR_EN;

        // Enable SPI TX/RX request
        m_Timeout = m_pDevice->TimeOut;
        m_pPort->pSPIx->CR2 |= (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

        do
        {
            nOS_Yield();

            if(m_pPort->HardwarePort < 4) bTransmitCompleted = (m_pPort->pDMA->LISR & m_pPort->DMA_Flag_TC_RX) ? true : false;
            else                          bTransmitCompleted = (m_pPort->pDMA->HISR & m_pPort->DMA_Flag_TC_RX) ? true : false;
            m_Timeout--;
        }
        while((m_Timeout > 0) && (bTransmitCompleted == false));

        // Disable DMA RX & TX Channel
        m_pPort->pDMA_RX->CR &= (uint16_t)(~DMA_SxCR_EN);
        m_pPort->pDMA_TX->CR &= (uint16_t)(~DMA_SxCR_EN);

        // Disable SPIx RX/TX request
        m_pPort->pSPIx->CR2 &= (uint16_t)~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);

        this->Unlock();

        if(bTransmitCompleted != true) return SYS_TIME_OUT;

      #else // SPI_USE_DMA_TRANSFERT == 0

        this->Lock();

        switch(Request)
        {
            case ACCESS_READ:
            {
                //dummy read to clear data register
                ((uint8_t*)pBuffer)[0] = m_pPort->pSPIx->DR;

                for(size_t i = 0; i < Size; i++)
                {
                    m_pPort->pSPIx->DR = 0xFF;
                    if(WaitReady() == false) return SYS_HUNG;
                    ((uint8_t*)pBuffer)[i] = m_pPort->pSPIx->DR;
                }
                break;
            }
            case ACCESS_WRITE:
            {
                for(size_t i = 0; i < Size; i++)
                {
                    m_pPort->pSPIx->DR = ((uint8_t*)pBuffer)[i];
                    WaitReady();
                }
                break;
            }
        }

        this->Unlock();

      #endif
        return SYS_READY;
    }
    return SYS_DEVICE_NOT_PRESENT;
}


//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    WaitReady
//
//  Description:    This function verify if the SPI is ready
//
//  Note(s):        return  SYS_READY if available
//                          SYS_HUNG  if busy pass timeout
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::WaitReady(void)
{

    m_Timeout = m_pDevice->TimeOut;
    while((m_pPort->pSPIx->SR & SPI_I2S_FLAG_TXE)  == 0)                // Wait until transmit complete
    {
        nOS_Yield();
        if(m_Timeout == 0) return SYS_HUNG;
    }

    m_Timeout = m_pDevice->TimeOut;
    while((m_pPort->pSPIx->SR & SPI_I2S_FLAG_RXNE) == 0)                // Wait until receive complete
    {
        nOS_Yield();
        if(m_Timeout == 0) return SYS_HUNG;
    }

    m_Timeout = m_pDevice->TimeOut;
    while((m_pPort->pSPIx->SR & SPI_I2S_FLAG_BSY)  != 0)                // wait until SPI is not busy anymore
    {
        nOS_Yield();
        if(m_Timeout == 0) return SYS_HUNG;
    }

    return SYS_READY;
}


//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    IRQHandler
//
//  Description:    This function handles SPIx interrupt request.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::IRQHandler(void)
{
    if(m_pPort->CallBackISR != nullptr)
    {
        m_pPort->CallBackISR();
    }
}

//-------------------------------------------------------------------------------------------------

#endif // USE_SPI_DRIVER == DEF_ENABLED
