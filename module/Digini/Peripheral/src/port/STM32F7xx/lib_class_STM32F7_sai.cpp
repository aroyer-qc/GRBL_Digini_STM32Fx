//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_sai.cpp
//
//*************************************************************************************************

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "digini_cfg.h"
#include <stdint.h>
#define STM32F7_SAI_GLOBAL
#include "lib_class_sai.h"
#undef  STM32F7_SAI_GLOBAL
#include "lib_class_gpio.h"
#include "lib_macro.h"
#include "bsp.h"
#include "assert.h"

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   SAI
//
//   Parameter(s):  SAI_PortInfo_t*  pPort
//
//   Description:   Initialize the SAIx peripheral according to the specified Parameters
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SAI_Driver::SAI_Driver(const SAI_PortInfo_t* pPort)
{
    m_pPort = const_cast<SAI_PortInfo_t*>(pPort);
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
SystemState_e SAI_Driver::GetStatus(void)
{
    return m_Status;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      GetFrequency
//
//   Parameter(s):  None
//   Return Value:  uint32_t
//
//   Description:   Get
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
uint32_t SAI_Driver::GetFrequency(void)
{
     return m_AudioFrequency;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SetFrequency
//
//   Parameter(s):  AudioFrequency
//   Return Value:  None
//
//   Description:   Set
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SAI_Driver::SetFrequency(uint32_t AudioFrequency)
{
    m_AudioFrequency = AudioFrequency;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SetAudioFrameSlot
//
//   Parameter(s):  None
//   Return Value:  uint32_t
//
//   Description:   Return general status of the driver
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void SAI_Driver::SetAudioFrameSlot(uint32_t AudioFrameSlot)
{
    // Disable SAI peripheral to allow access to SAI internal registers
    m_pPort->pSAI_BlockOut->CR1 &= ~SAI_xCR1_SAIEN;

    // Update the SAI audio frame slot configuration
    MODIFY_REG(m_pPort->pSAI_BlockOut->SLOTR, SAI_xSLOTR_SLOTEN_Pos, AudioFrameSlot);

    // Enable SAI peripheral to generate MCLK
    m_pPort->pSAI_BlockOut->CR1 |= SAI_xCR1_SAIEN;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      SetAudioFrameSlot
//
//   Parameter(s):  pData    Pointer to data buffer
//                  Size    Amount of data to be sent
//   Return Value:  SystemState_e
//
//   Description:   Transmit an amount of data in non-blocking mode with DMA.
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
SystemState_e SAI_Driver::Transmit_DMA(uint8_t* pData, uint16_t Size)
{
    if((pData == nullptr) || (Size == 0))
    {
        return SYS_ERROR;
    }

    m_pPort->pDMA->NDTR = Size;                                                    // Configure DMA Stream data length
    m_pPort->pDMA->PAR  = uint32_t(&m_pPort->pSAI_BlockOut->DR);                // Configure DMA Stream source address
    m_pPort->pDMA->M0AR = uint32_t(pData);                                        // Configure DMA Stream destination address

    CLEAR_BIT(m_pPort->pDMA->CR, DMA_SxCR_DBM);                                    // Clear DBM bit
    SET_BIT(m_pPort->pDMA->CR, DMA_SxCR_TCIE |
                               DMA_SxCR_HTIE |
                               DMA_SxCR_TEIE |
                               DMA_SxCR_DMEIE);                                    // Enable Common interrupts
    SET_BIT(m_pPort->pDMA->FCR, DMA_IT_FE);

    SET_BIT(m_pPort->pDMA->CR, DMA_SxCR_EN);
//    SET_BIT(m_pPort->pSAI_BlockOut->CR1, SAI_xCR1_SAIEN);

    // Enable the interrupts for error handling
    SET_BIT(m_pPort->pSAI_BlockOut->IMR, SAI_xIMR_OVRUDRIE | SAI_xIMR_WCKCFGIE);


    // Enable SAI TX DMA Request
    SET_BIT(m_pPort->pSAI_BlockOut->CR1, SAI_xCR1_DMAEN);

    return SYS_READY;
}

//-------------------------------------------------------------------------------------------------

void SAI_Driver::StopDMA(void)
{
    CLEAR_BIT(m_pPort->pDMA->CR, DMA_SxCR_EN);
    CLEAR_BIT(m_pPort->pSAI_BlockOut->CR1, SAI_xCR1_DMAEN);
}
