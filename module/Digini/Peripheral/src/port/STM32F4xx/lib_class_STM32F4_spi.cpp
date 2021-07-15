//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_spi.cpp
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

#define SPI_DRIVER_GLOBAL
#include "lib_class_STM32F4_spi.h"
#undef  SPI_DRIVER_GLOBAL
#include "lib_utility.h"
#include "lib_macro.h"

//-------------------------------------------------------------------------------------------------

#if (USE_SPI_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define SPI_DMA_TRANSFER_TIMEOUT            1000

#define SPI_DMA_SxCR_8_OR_16_BITS_MASK        DMA_SxCR_MSIZE | DMA_SxCR_PSIZE
#define SPI_DMA_SxCR_8_BITS_CFG               0
#define SPI_DMA_SxCR_16_BITS_CFG              DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

/*
typedef struct HAL_SPI_DMA_Data_s
{
	volatile HAL_SPI_DMA_Status_te Status;
    const HALISR_Prio_ts*          pIsrPrioTX;
    const HALISR_Prio_ts*          pIsrPrioRX;
    HAL_SPI_Speed_te               Speed;
} HAL_SPI_DMA_Data_ts;
*/

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
//   Parameter(s):  SPI_ID          ID of the SPI info
//
//   Description:   Initializes the SPIx peripheral according to the specified Parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SPI_Driver::SPI_Driver(SPI_ID_e SPI_ID)
{
    m_pDevice        = nullptr;
    m_pInfo          = &SPI_Info[SPI_ID];
    m_Status         = SYS_DEVICE_NOT_PRESENT;
    m_pInfo->pObject = this;
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
    ISR_Prio_t          ISR_Prio;
    uint32_t            PriorityGroup;

  #if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
    DMA_Stream_TypeDef* pDMA;
  #endif

    nOS_MutexCreate(&m_Mutex, NOS_MUTEX_RECURSIVE, NOS_MUTEX_PRIO_INHERIT);

    // ---- Configure and enable SPI interrupt ----
    //uint32_t Priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), m_pPort->PreempPrio, 0x00);
    //NVIC_SetPriority(m_pPort->IRQn, Priority);
    //NVIC_EnableIRQ(m_pPort->IRQn);

    IO_PinInit(m_pInfo->PinCLK);
    IO_PinInit(m_pInfo->PinMOSI);
    IO_PinInit(m_pInfo->PinMISO);
    IO_PinInit(m_pInfo->PinNSS);

    switch(uint32_t(m_pInfo->pSPIx))
    {
      #if (SPI_DRIVER_SUPPORT_SPI1 == DEF_ENABLE)
        case uint32_t(SPI1):
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB2RSTR |=  RCC_APB2RSTR_SPI1RST;             // Enable SPI1 reset state
            RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;             // Release SPI1 from reset state
            RCC->APB2ENR  |=  RCC_APB2ENR_SPI1EN;               // Enable SPI_PORT clock
        }
        break;
      #endif

      #if (SPI_DRIVER_SUPPORT_SPI2 == DEF_ENABLE)
        case uint32_t(SPI2):
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB1RSTR |=  RCC_APB1RSTR_SPI2RST;             // Enable SPI2 reset state
            RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;             // Release SPI2 from reset state
            RCC->APB1ENR  |=  RCC_APB1ENR_SPI2EN;               // Enable SPI_PORT clock
        }
        break;
      #endif

      #if (SPI_DRIVER_SUPPORT_SPI3 == DEF_ENABLE)
        case uint32_t(SPI3):
        {
            // ---- Reset peripheral and set clock ----
            RCC->APB1RSTR |=  (RCC_APB1RSTR_SPI3RST;            // Enable sPI3 reset state
            RCC->APB1RSTR &= ~(RCC_APB1RSTR_SPI3RST;            // Release SPI3 from reset state
            RCC->APB1ENR  |=  (RCC_APB1ENR_SPI3EN;              // Enable SPI_PORT clock
        }
        break;
      #endif
    }

    //---------------------------- SPIx CR2 Configuration ------------------------
    // Change this in futur, so if the pin NSS is set as a function of the SPI..
    // MODIFY_REG(pSPIx->CR2, SPI_CR2_SSOE, (SPI_NSS_HARD_INPUT >> 16U));   ??

    // Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register)
    //CLEAR_BIT(pSPIx->I2SCFGR, SPI_I2SCFGR_I2SMOD);


    //---------------------------- SPIx CR1 Configuration and enable module ------

    Config(SPI_CFG_CR1_CLEAR_MASK, SPI_FULL_DUPLEX      |
                                   SPI_MODE_MASTER      |
                                   SPI_DATA_WIDTH_8_BIT |
                                   SPI_POLARITY_LOW     |
                                   SPI_PHASE_1_EDGE     |
                                   SPI_NSS_SOFT         |
                                   SPI_MSB_FIRST        |
                                   m_pInfo->Speed);

  #if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
    RCC->AHB1ENR |= m_pInfo->RCC_AHBxPeriph;            // Initialize DMA clock
    m_DMA_Status  = SYS_IDLE;
    m_NoMemoryIncrement == false;

    // Preinit register that won't change
    pDMA = m_pInfo->DMA_StreamTX;
    pDMA->PAR = (uint32_t)&m_pInfo->pSPIx->DR;          // Configure transmit data register
    pDMA->CR = DMA_MEMORY_TO_PERIPH           |
               DMA_MODE_NORMAL                |
               DMA_PERIPH_NO_INCREMENT        |
               DMA_MEMORY_INCREMENT           |
               DMA_P_DATA_ALIGN_BYTE          |
               DMA_M_DATA_ALIGN_BYTE          |
               DMA_P_BURST_SINGLE             |
               DMA_M_BURST_SINGLE             |
               DMA_PRIORITY_LOW               |
               DMA_SxCR_TCIE                  |
               m_pInfo->DMA_ChannelTX;

    pDMA = m_pInfo->DMA_StreamRX;
    pDMA->PAR = (uint32_t)&m_pInfo->pSPIx->DR;          // Configure receive data register
    pDMA->CR = DMA_PERIPH_TO_MEMORY           |
               DMA_MODE_NORMAL                |
               DMA_PERIPH_NO_INCREMENT        |
               DMA_MEMORY_INCREMENT           |
               DMA_P_DATA_ALIGN_BYTE          |
               DMA_M_DATA_ALIGN_BYTE          |
               DMA_P_BURST_SINGLE             |
               DMA_M_BURST_SINGLE             |
               DMA_PRIORITY_LOW               |
               DMA_SxCR_TCIE                  |
               m_pInfo->DMA_ChannelRX;
  #endif

    PriorityGroup = NVIC_GetPriorityGrouping();
    ISR_Prio.PriorityGroup = PriorityGroup;
    ISR_Prio.SubPriority   = 0;

    // NVIC Setup for TX DMA channels interrupt request
    ISR_Prio.PremptionPriority = 6;
    ISR_Init(m_pInfo->TX_IRQn, &ISR_Prio);

    // NVIC Setup for RX DMA channels interrupt request
    ISR_Prio.PremptionPriority = 6;
    ISR_Init(m_pInfo->RX_IRQn, &ISR_Prio);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           LockToDevice
//
//  Parameter(s):   void*          pDevice
//  Return:         SystemState_e  Status
//
//  Description:    This routine will configure the SPI port to work with a specific device and
//                  lock it, so any other access to the port will be block until unlock
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::LockToDevice(void* pDevice)
{
    if(m_pDevice == nullptr)
    {
        while(nOS_MutexLock(&m_Mutex, NOS_WAIT_INFINITE) != NOS_OK){};
        m_pDevice = pDevice;
        m_Status  = SYS_READY;
    }

    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           UnlockFromDevice
//
//  Parameter(s):   void*         pDevice
//  Return:         SystemState_e Status
//
//  Description:    This routine will unlock SPI port from a specific device
//
//  Note(s):        If a write without lock is executed then it will be done on the locked device
//                  if lock and no write at all if not lock to a device
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::UnlockFromDevice(void* pDevice)
{
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
/*
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
*/

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
//  Parameter(s):   SPI_Speed_e     Speed
//  Return:         None
//
//  Description:    Set speed on the SPI port
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::Config(uint32_t Mask, uint32_t Config)
{
    CLEAR_BIT(m_pInfo->pSPIx->CR1, SPI_CR1_SPE);                // Disable SPIx
    MODIFY_REG(m_pInfo->pSPIx->CR1, Mask, Config);
    SET_BIT(m_pInfo->pSPIx->CR1, SPI_CR1_SPE);                  // Enable SPIx
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Send a single byte
//
//  Parameter(s):   Data        Single byte to send
//  Return:         None
//
//  Description:    Send a byte to SPI
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
uint8_t SPI_Driver::Send(uint8_t Data)
{
    SPI_TypeDef* pSPIx = m_pInfo->pSPIx;

    uint16_t u16_Data = Data;

    while((pSPIx->SR & SPI_SR_TXE) == 0);
    pSPIx->DR = u16_Data;
    while((pSPIx->SR & SPI_SR_RXNE) == 0);                  // Wait to receive a byte
    u16_Data = pSPIx->DR;
    Data = (uint8_t)u16_Data;

    return Data;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   pTX_Data        Pointer on the data buffer to transfer.
//                  TX_Size			Number of byte to send.
//                  pRX_pData       Pointer on the data buffer where to put received data.
//                  RX_Size			Number of byte to receive.
//                  pDevice         Pointer of the device using SPI (it's a unique ID in the system)
//  Return:         SystemState_e   State
//
//  Description:    Read or writes data to SPI device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
SystemState_e SPI_Driver::Transfer(const uint8_t* pTX_Data, uint32_t TX_Size, uint8_t* pRX_Data, uint32_t RX_Size, void* pDevice)
{
    SystemState_e State;

    if((State = LockToDevice(pDevice)) == SYS_READY)
    {
        State = Transfer(pTX_Data, TX_Size, pRX_Data, RX_Size);
        UnlockFromDevice(pDevice);
    }

    return State;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   pTX_Data        Pointer on the data buffer to transfer.
//                  TX_Size			Number of byte to send.
//                  pRX_pData       Pointer on the data buffer where to put received data.
//                  RX_Size			Number of byte to receive.
//  Return:         SystemState_e   State
//
//  Description:    Read or writes data to SPI device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
SystemState_e SPI_Driver::Transfer(const uint8_t* pTX_Data, uint32_t TX_Size, uint8_t* pRX_Data, uint32_t RX_Size)
{
	SPI_TypeDef*              pSPIx;
    DMA_Stream_TypeDef*       pDMA;
    uint32_t                  Flag;
    uint32_t                  Dummy;

    if(m_pDevice != nullptr)
    {
        if(((pTX_Data == NULL) || (TX_Size == 0)) &&
           ((pRX_Data == NULL) || (RX_Size == 0)))
        {
            return SYS_NULLPTR;
        }

        if(m_Status == SYS_READY)
        {
            CLEAR_BIT(m_pInfo->pSPIx->CR1, SPI_CR1_SPE);

            if(m_pInfo->PinNSS != IO_NOT_DEFINED)
            {
                IO_SetPinLow(m_pInfo->PinNSS);                              // Select the NSS pin
            }

            // ----------------------------------------------------------------------------------------
            // TX Setup

            // Note(s) needed by both TX and RX
            pDMA  = m_pInfo->DMA_StreamTX;                                  // In all case the first DMA set is the TX
            pSPIx = m_pInfo->pSPIx;
            Flag  = m_pInfo->TX_IT_Flag;

            if((pTX_Data != NULL) && (TX_Size != 0))
            {
                // TX DMA
                m_DMA_Status = SYS_BUSY_TX;                                 // Set flag to busy in TX
                pDMA->M0AR = ((uint32_t)pTX_Data);                          // Set DMA source
                pDMA->NDTR = TX_Size;                                       // Set size of the TX

                if(m_NoMemoryIncrement == false)
                {
                    SET_BIT(pDMA->CR, DMA_SxCR_MINC);                       // Enable transfer complete and memory increment
                }
                else
                {
                    CLEAR_BIT(pDMA->CR, DMA_SxCR_MINC);
                    m_NoMemoryIncrement == false;
                }

                SET_BIT(pDMA->CR, DMA_SxCR_EN);                             // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);                                  // Clear IRQ DMA flag

                // SPI
                if(m_pInfo->Control == SPI_HALF_DUPLEX)
                {
                    MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_HALF_DUPLEX_TX);
                }

                SET_BIT(pSPIx->CR1, SPI_CR1_SPE);                           // Enable SPI
                SET_BIT(pSPIx->CR2, SPI_CR2_TXDMAEN);                       // Enable DMA TX

                if(WaitDMA() == SYS_ERROR)
                {
                    if(m_pInfo->PinNSS != IO_NOT_DEFINED)
                    {
                        IO_SetPinHigh(m_pInfo->PinNSS);
                    }

                    return SYS_TIME_OUT;
                }

                CLEAR_BIT(pSPIx->CR1, SPI_CR1_SPE);                         // Disable SPI

                // Deactivate DMA
                CLEAR_BIT(pSPIx->CR2, SPI_CR2_TXDMAEN);
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                           // Disable the DMA module
            }

            // ----------------------------------------------------------------------------------------
            // RX setup

            if((pRX_Data != NULL) && (RX_Size != 0))
            {

                while((pSPIx->SR & SPI_SR_RXNE) != 0)
                {
                    Dummy = pSPIx->DR;
                }

                m_DMA_Status = SYS_BUSY_RX;                                 // Set flag to busy in TX

                // TX DMA
                Dummy = 0xFF;                                               // Value to dummy TX
                pDMA->M0AR = (uint32_t)&Dummy;                              // Set DMA source for dummy TX
                pDMA->NDTR = RX_Size;                                       // Set size of the TX
                CLEAR_BIT(pDMA->CR, DMA_SxCR_MINC);
                SET_BIT(pDMA->CR, DMA_SxCR_EN);                             // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);                                  // Clear IRQ DMA flag
                // RX DMA
                pDMA = m_pInfo->DMA_StreamRX;                               // Now configure RX DMA
                Flag = m_pInfo->RX_IT_Flag;
                pDMA->M0AR = (uint32_t)pRX_Data;
                pDMA->NDTR = RX_Size;
                SET_BIT(pDMA->CR, DMA_SxCR_EN);                             // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);

                // SPI
                if(m_pInfo->Control == SPI_HALF_DUPLEX)
                {
                    MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_HALF_DUPLEX_RX);
                }

                SET_BIT(pSPIx->CR1, SPI_CR1_SPE);                           // Enable SPI
                SET_BIT(pSPIx->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));   // Start the process

                if(WaitDMA() == SYS_ERROR)
                {
                    if(m_pInfo->PinNSS != IO_NOT_DEFINED)
                    {
                        IO_SetPinHigh(m_pInfo->PinNSS);                     // There is an error then release NSS
                    }

                    return SYS_TIME_OUT;
                }


                // Deactivate DMA
                CLEAR_BIT(pSPIx->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                           // Disable the DMA RX module
                pDMA = m_pInfo->DMA_StreamTX;                               // In all case the first DMA set is the TX
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                           // Disable the DMA TX module
            }

            // ----------------------------------------------------------------------------------------

            CLEAR_BIT(pSPIx->CR1, SPI_CR1_SPE);                             // Disable SPI
            if(m_pInfo->Control == SPI_HALF_DUPLEX)
            {
                MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_FULL_DUPLEX);
            }


            if(m_pInfo->PinNSS != IO_NOT_DEFINED)
            {
                IO_SetPinHigh(m_pInfo->PinNSS);                             // Select the NSS pin
            }

            m_DMA_Status = SYS_IDLE;
            return SYS_READY;
        }

        return SYS_BUSY;
    }

    return SYS_DEVICE_NOT_PRESENT;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   pTX_Data        Pointer on the data buffer (uint16_t*) to transfer.
//                  TX_Size			Number of byte to send.
//                  pRX_pData       Pointer on the data buffer (uint16_t*) where to put received data.
//                  RX_Size			Number of byte to receive.
//                  pDevice         Pointer of the device using SPI (it's a unique ID in the system)
//  Return:         SystemState_e   State
//
//  Description:    Read or writes data to SPI device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
SystemState_e SPI_Driver::Transfer(const uint16_t* pTX_Data, uint32_t TX_Size, uint16_t* pRX_Data, uint32_t RX_Size, void* pDevice)
{
    SystemState_e State;

    if((State = LockToDevice(pDevice)) == SYS_READY)
    {
        State = Transfer(pTX_Data, TX_Size, pRX_Data, RX_Size);
        UnlockFromDevice(pDevice);
    }

    return State;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   pTX_Data        Pointer on the data buffer (uint16_t*) to transfer.
//                  TX_Size			Number of byte to send.
//                  pRX_pData       Pointer on the data buffer (uint16_t*) where to put received data.
//                  RX_Size			Number of byte to receive.
//  Return:         SystemState_e   State
//
//  Description:    Read or writes data to SPI device.
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
SystemState_e SPI_Driver::Transfer(const uint16_t* pTX_Data, uint32_t TX_Size, uint16_t* pRX_Data, uint32_t RX_Size)
{
	SPI_TypeDef*              pSPIx;
    DMA_Stream_TypeDef*       pDMA;
    uint32_t                  Flag;
    uint32_t                  Dummy;

    if(m_pDevice != nullptr)
    {
        if(((pTX_Data == NULL) || (TX_Size == 0)) &&
           ((pRX_Data == NULL) || (RX_Size == 0)))
        {
            return SYS_NULLPTR;
        }

        if(m_Status == SYS_READY)
        {
            CLEAR_BIT(m_pInfo->pSPIx->CR1, SPI_CR1_SPE);

            if(m_pInfo->PinNSS != IO_NOT_DEFINED)
            {
                IO_SetPinLow(m_pInfo->PinNSS);                                                  // Select the NSS pin
            }

            // ----------------------------------------------------------------------------------------
            // TX Setup

            // Note(s) needed by both TX and RX
            pDMA  = m_pInfo->DMA_StreamTX;                                                      // In all case the first DMA set is the TX
            pSPIx = m_pInfo->pSPIx;
            Flag  = m_pInfo->TX_IT_Flag;

            if((pTX_Data != NULL) && (TX_Size != 0))
            {
                // TX DMA
                m_DMA_Status = SYS_BUSY_TX;                                                     // Set flag to busy in TX
                pDMA->M0AR = ((uint32_t)pTX_Data);                                              // Set DMA source
                pDMA->NDTR = TX_Size;                                                           // Set size of the TX
                MODIFY_REG(pDMA->CR, SPI_DMA_SxCR_8_OR_16_BITS_MASK, SPI_DMA_SxCR_16_BITS_CFG);

                if(m_NoMemoryIncrement == false)
                {
                    SET_BIT(pDMA->CR, DMA_SxCR_MINC);                                           // Enable transfer complete and memory increment
                }
                else
                {
                    CLEAR_BIT(pDMA->CR, DMA_SxCR_MINC);
                    m_NoMemoryIncrement = false;
                }

                SET_BIT(pDMA->CR, DMA_SxCR_EN);                                                 // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);                                                      // Clear IRQ DMA flag

                // SPI
                if(m_pInfo->Control == SPI_HALF_DUPLEX)
                {
                    MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_HALF_DUPLEX_TX);
                }

                SET_BIT(pSPIx->CR1, SPI_CR1_DFF | SPI_CR1_SPE);                                 // Enable SPI at 16 Bits
                SET_BIT(pSPIx->CR2, SPI_CR2_TXDMAEN);                                           // Enable DMA TX

                if(WaitDMA() == SYS_ERROR)
                {
                    if(m_pInfo->PinNSS != IO_NOT_DEFINED)
                    {
                        IO_SetPinHigh(m_pInfo->PinNSS);
                    }

                    return SYS_TIME_OUT;
                }

                CLEAR_BIT(pSPIx->CR1, SPI_CR1_DFF | SPI_CR1_SPE);                               // Disable SPI and put back to 8 Bits default frame format

                // Deactivate DMA
                CLEAR_BIT(pSPIx->CR2, SPI_CR2_TXDMAEN);
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                                               // Disable the DMA module
            }

            // ----------------------------------------------------------------------------------------
            // RX setup

            if((pRX_Data != NULL) && (RX_Size != 0))
            {

                while((pSPIx->SR & SPI_SR_RXNE) != 0)
                {
                    Dummy = pSPIx->DR;
                }

                m_DMA_Status = SYS_BUSY_RX;                                                     // Set flag to busy in TX

                // TX DMA
                Dummy = 0xFF;                                                                   // Value to dummy TX
                pDMA->M0AR = (uint32_t)&Dummy;                                                  // Set DMA source for dummy TX
                pDMA->NDTR = RX_Size;                                                           // Set size of the TX
                MODIFY_REG(pDMA->CR, SPI_DMA_SxCR_8_OR_16_BITS_MASK, SPI_DMA_SxCR_16_BITS_CFG); // Set 16 Bits Transfer
                CLEAR_BIT(pDMA->CR, DMA_SxCR_MINC);
                SET_BIT(pDMA->CR, DMA_SxCR_EN);                                                 // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);                                                      // Clear IRQ DMA flag

                // RX DMA
                pDMA = m_pInfo->DMA_StreamRX;                                                   // Now configure RX DMA
                Flag = m_pInfo->RX_IT_Flag;
                pDMA->M0AR = (uint32_t)pRX_Data;
                pDMA->NDTR = RX_Size;
                MODIFY_REG(pDMA->CR, SPI_DMA_SxCR_8_OR_16_BITS_MASK, SPI_DMA_SxCR_16_BITS_CFG); // Set 16 Bits Transfer
                SET_BIT(pDMA->CR, DMA_SxCR_EN);                                                 // Enable the DMA module
                DMA_ClearFlag(pDMA, Flag);

                // SPI
                if(m_pInfo->Control == SPI_HALF_DUPLEX)
                {
                    MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_HALF_DUPLEX_RX);
                }
                SET_BIT(pSPIx->CR1, SPI_CR1_SPE);                                               // Enable SPI
                SET_BIT(pSPIx->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));                       // Start the process

                if(WaitDMA() == SYS_ERROR)
                {
                    if(m_pInfo->PinNSS != IO_NOT_DEFINED)
                    {
                        IO_SetPinHigh(m_pInfo->PinNSS);                                         // There is an error then release NSS
                    }

                    return SYS_TIME_OUT;
                }

                // Deactivate DMA
                CLEAR_BIT(pSPIx->CR2, (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN));
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                                               // Disable the DMA RX module
                MODIFY_REG(pDMA->CR, SPI_DMA_SxCR_8_OR_16_BITS_MASK, SPI_DMA_SxCR_8_BITS_CFG);  // Put back to 8 bits default
                pDMA = m_pInfo->DMA_StreamTX;                                                   // In all case the first DMA set is the TX
                CLEAR_BIT(pDMA->CR, DMA_SxCR_EN);                                               // Disable the DMA TX module
            }

            MODIFY_REG(pDMA->CR, SPI_DMA_SxCR_8_OR_16_BITS_MASK, SPI_DMA_SxCR_8_BITS_CFG);      // Put back to 8 bits default for TX ( also use on RX

            // ----------------------------------------------------------------------------------------

            CLEAR_BIT(pSPIx->CR1, SPI_CR1_SPE);                                                 // Disable SPI
            if(m_pInfo->Control == SPI_HALF_DUPLEX)
            {
                MODIFY_REG(pSPIx->CR1, SPI_DUPLEX_MASK, SPI_FULL_DUPLEX);
            }


            if(m_pInfo->PinNSS != IO_NOT_DEFINED)
            {
                IO_SetPinHigh(m_pInfo->PinNSS);                                                 // Select the NSS pin
            }

            m_DMA_Status = SYS_IDLE;
            return SYS_READY;
        }

        return SYS_BUSY;
    }

    return SYS_DEVICE_NOT_PRESENT;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Name:           ChipSelect
//
//  Description:    This function verify if the SPI is ready
//
//  Parameter(s):   bool IsItActive
//
//  Return:         None
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::ChipSelect(bool IsItActive)
{
    if(IsItActive == true)
    {
        IO_SetPinLow(m_pInfo->PinNSS);                      // Select the NSS pin
    }
    else
    {
        while((m_pInfo->pSPIx->SR & SPI_SR_BSY) != 0)                // Wait for busy to clear
        {
                nOS_Yield();
        }

        IO_SetPinHigh(m_pInfo->PinNSS);                     // De select the NSS pin
    }
}

//-------------------------------------------------------------------------------------------------
//  Name:           OverrideMemoryIncrement
//
//  Description:    This set override memory increment for DMA transfer.
//
//  Parameter(s):   None
//
//  Return:         None
//
//  Note(s):        This is valid only for the next transaction
//
//-------------------------------------------------------------------------------------------------
void SPI_Driver::OverrideMemoryIncrement(void)
{
    m_NoMemoryIncrement = true;
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

    m_Timeout = 100;
    while((m_pInfo->pSPIx->SR & SPI_SR_TXE)  == 0)          // Wait until transmit complete
    {
        nOS_Yield();
        if(m_Timeout == 0) return SYS_HUNG;
    }

    m_Timeout = 100;
    while((m_pInfo->pSPIx->SR & SPI_SR_RXNE) == 0)          // Wait until receive complete
    {
        nOS_Yield();
        if(m_Timeout == 0) return SYS_HUNG;
    }

    m_Timeout = 100;
    while((m_pInfo->pSPIx->SR & SPI_SR_BSY)  != 0)          // wait until SPI is not busy anymore
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
    //if(m_pInfo->CallBackISR != nullptr)
    //{
    //    m_pInfo->CallBackISR();
    //}
}

//-------------------------------------------------------------------------------------------------
//  DDDDDD  MM     MM  AAAAA
//  DD   DD MMMM MMMM AA   AA
//  DD   DD MM MMM MM AAAAAAA
//  DD   DD MM  M  MM AA   AA
//  DDDDDD  MM     MM AA   AA
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   None
//  Return:         SystemState_e   State
//
//  Description:    Get status of the SPI DMA transfer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SPI_Driver::WaitDMA(void)
{
    nOS_TickCounter   TickStart;
    SPI_TypeDef*      pSPIx;

    pSPIx = m_pInfo->pSPIx;

    // Wait for DMA to conclude
    TickStart = GetTick();
    while(m_DMA_Status != SYS_BUSY_B4_RELEASE)
    {
        if(TickHasTimeOut(TickStart, SPI_DMA_TRANSFER_TIMEOUT) == true)
        {
            return SYS_TIME_OUT;
        }

        nOS_Yield();
    };

    // Wait for SPI to conclude
    TickStart = GetTick();
    while((pSPIx->SR & SPI_SR_BSY) != 0)
    {
        if(TickHasTimeOut(TickStart, SPI_DMA_TRANSFER_TIMEOUT) == true)
        {
            return SYS_TIME_OUT;
        }

        nOS_Yield();
    };

    return SYS_IDLE;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   None
//  Return:         SystemState_e   State
//
//  Description:    Get status of the SPI DMA transfer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
void SPI_Driver::DMA_TX_IRQ_Handler(SPI_ID_e SPI_ID)
{
    SPI_Info_t*         pInfo;
    DMA_Stream_TypeDef* pDMA;
    uint32_t            Flag;

    pInfo = &SPI_Info[SPI_ID];
    pInfo->pObject->m_DMA_Status = SYS_BUSY_B4_RELEASE;
    pDMA = pInfo->DMA_StreamTX;
    Flag = pInfo->TX_IT_Flag;
    DMA_ClearFlag(pDMA, Flag);
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           Transfer
//
//  Parameter(s):   None
//  Return:         SystemState_e   State
//
//  Description:    Get status of the SPI DMA transfer
//
//  Note(s):
//
//-------------------------------------------------------------------------------------------------
#if (SPI_DRIVER_SUPPORT_DMA == DEF_ENABLED)
void SPI_Driver::DMA_RX_IRQ_Handler(SPI_ID_e SPI_ID)
{
    SPI_Info_t*         pInfo;
    DMA_Stream_TypeDef* pDMA;
    uint32_t            Flag;

    pInfo = &SPI_Info[SPI_ID];
    pInfo->pObject->m_DMA_Status = SYS_BUSY_B4_RELEASE;
    pDMA = pInfo->DMA_StreamRX;
    Flag = pInfo->RX_IT_Flag;
    DMA_ClearFlag(pDMA, Flag);
}
#endif

//-------------------------------------------------------------------------------------------------

/*
uint32_t SPI_Driver::GetDMA_Status(void)
{
	return m_DMA_Status;
}
*/


//-------------------------------------------------------------------------------------------------

#endif // USE_SPI_DRIVER == DEF_ENABLED
