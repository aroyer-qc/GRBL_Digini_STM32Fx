//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F4_uart.cpp
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
#include "lib_macro.h"
#define UART_DRIVER_GLOBAL
#include "lib_class_STM32F4_uart.h"
#undef  UART_DRIVER_GLOBAL
//#include "clock_cfg.h"

//-------------------------------------------------------------------------------------------------

#if (USE_UART_DRIVER == DEF_ENABLED)

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define UART_BACK_OFFSET_RESET_REGISTER     0x20

#define UART_CR1_PARITY_NONE                0                                   // Parity control disabled
#define UART_CR1_PARITY_EVEN                USART_CR1_PCE                       // Parity control enabled and Even Parity is selected
#define UART_CR1_PARITY_ODD                 (USART_CR1_PCE | USART_CR1_PS)      // Parity control enabled and Odd Parity is selected

#define UART_CR1_DATA_WIDTH_8B              0                                   // 8 bits word length
#define UART_CR1_DATA_WIDTH_9B              USART_CR1_M                         // 9 bits word length

#define UART_CR1_OVERSAMPLING_16            0                                   // Oversampling by 16
#define UART_CR1_OVERSAMPLING_8             USART_CR1_OVER8                     // Oversampling by 8

#define UART_CR1_OVER_16                    0
#define UART_CR1_OVER_8                     USART_CR1_OVER8

#define UART_CR1_RX                         USART_CR1_RE
#define UART_CR1_TX                         USART_CR1_TE
#define UART_CR1_RX_TX                      USART_CR1_RE | USART_CR1_TE

#define UART_CR2_STOP_1B                    0                                   // 1   stop bit
#define UART_CR2_STOP_0_5B                  USART_CR2_STOP_0                    // 0.5 stop bit
#define UART_CR2_STOP_2_B                   USART_CR2_STOP_1                    // 2   stop bits
#define UART_CR2_STOP_1_5B                  USART_CR2_STOP_0 | USART_CR2_STOP_1 // 1.5 stop bits

#define UART_CR3_FLOW_CTS                   USART_CR3_CTSE
#define UART_CR3_FLOW_CTS_ISR               USART_CR3_CTSIE
#define UART_CR3_FLOW_RTS                   USART_CR3_RTSE

//-------------------------------------------------------------------------------------------------
//  private variable(s)
//-------------------------------------------------------------------------------------------------

const uint32_t UART_Driver::m_BaudRate[NB_OF_BAUD] =
{
    9600,
    19200,
    38400,
    57600,
    115200,
    230400,
    460800,
    921600,
    1843200,
};

//-------------------------------------------------------------------------------------------------
//
//   Class: UART_Driver
//
//
//   Description:   Class to handle UART
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Constructor:   UART_Driver
//
//   Parameter(s):  UartID                  ID for the data to use for this class
//   Return Value:
//
//   Description:   Initializes the UART_Driver class
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
UART_Driver::UART_Driver(UART_ID_e UartID)
{
    ISR_Prio_t          ISR_Prio;
    uint32_t            PriorityGroup;
    DMA_Stream_TypeDef* pDMA;

  #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
    m_pDMA_Info      = nullptr;
    m_DMA_IsItBusyTX = false;
  #endif

    PriorityGroup = NVIC_GetPriorityGrouping();
    ISR_Prio.PriorityGroup     = PriorityGroup;
    ISR_Prio.SubPriority       = 0;
    ISR_Prio.PremptionPriority = m_pInfo->PreempPrio;

    if(UartID < NB_OF_REAL_UART_DRIVER)
    {
        m_UartID = UartID;
        m_pInfo = (UART_Info_t*)&UART_Info[UartID];
        m_pUart = m_pInfo->pUARTx;

      #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
        for(uint32_t i = 0; i < (uint32_t)NB_OF_UART_DMA_DRIVER; i++)
        {
            if(UART_DMA_Info[i].UartID == UartID)
            {
                m_pDMA_Info = (UART_DMA_Info_t*)&UART_DMA_Info[i];
                i = NB_OF_UART_DMA_DRIVER;
            }
        }

        if(m_pDMA_Info != nullptr)
        {
            SET_BIT(RCC->AHB1ENR, m_pDMA_Info->RCC_AHBxPeriph);                 // Initialize DMA clock

            pDMA = m_pDMA_Info->DMA_StreamRX;                                   // Write config that will never change
            pDMA->CR = DMA_MODE_NORMAL                    |
                       DMA_PERIPH_TO_MEMORY               |
                       DMA_PERIPH_NO_INCREMENT            |
                       DMA_MEMORY_INCREMENT               |
                       DMA_P_DATA_ALIGN_BYTE              |
                       DMA_M_DATA_ALIGN_BYTE              |
                       DMA_P_BURST_SINGLE                 |
                       DMA_M_BURST_SINGLE                 |
                       DMA_PRIORITY_HIGH;
            SET_BIT(pDMA->CR, m_pDMA_Info->DMA_ChannelRX);
            pDMA->PAR = (uint32_t)&m_pUart->DR;

            pDMA = m_pDMA_Info->DMA_StreamTX;                                   // Write config that will never change
            pDMA->CR = DMA_MEMORY_TO_PERIPH               |
                       DMA_MODE_NORMAL                    |
                       DMA_PERIPH_NO_INCREMENT            |
                       DMA_MEMORY_INCREMENT               |
                       DMA_P_DATA_ALIGN_BYTE              |
                       DMA_M_DATA_ALIGN_BYTE              |
                       DMA_P_BURST_SINGLE                 |
                       DMA_M_BURST_SINGLE                 |
                       DMA_PRIORITY_HIGH;
            SET_BIT(pDMA->CR, m_pDMA_Info->DMA_ChannelTX);
            pDMA->PAR  = (uint32_t)&m_pUart->DR;
        }
      #endif

        memset(&m_Variables, 0x00, sizeof(UART_Variables_t));

        *(uint32_t*)((uint32_t)m_pInfo->RCC_APBxEN_Register - UART_BACK_OFFSET_RESET_REGISTER) |=  m_pInfo->RCC_APBxPeriph;
        *(uint32_t*)((uint32_t)m_pInfo->RCC_APBxEN_Register - UART_BACK_OFFSET_RESET_REGISTER) &= ~m_pInfo->RCC_APBxPeriph;
        *(m_pInfo->RCC_APBxEN_Register) |= m_pInfo->RCC_APBxPeriph;

        this->SetConfig(m_pInfo->Config, m_pInfo->BaudID);

        IO_PinInit(m_pInfo->PinRX);
        IO_PinInit(m_pInfo->PinTX);
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_PinInit(m_pInfo->PinCTS);
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_PinInit(m_pInfo->PinRTS);
      #endif

      #if (UART_ISR_RX_CFG == DEF_ENABLED)
        m_pCallbackRX = nullptr;
        m_pContextRX  = nullptr;
      #endif
      #if UART_ISR_RX_IDLE_CFG == DEF_ENABLED
        m_pCallbackIDLE = nullptr;
        m_pContextIDLE  = nullptr;
      #endif
      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        m_pCallbackERROR = nullptr;
        m_pContextERROR  = nullptr;
      #endif
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        m_pCallbackCTS = nullptr;
        m_pContextCTS  = nullptr;
      #endif
      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        m_pCallbackEmptyTX = nullptr;
        m_pContextEmptyTX  = nullptr;
      #endif
      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        m_pCallbackCompletedTX = nullptr;
        m_pContextCompletedTX  = nullptr;
      #endif

        ISR_Init(m_pInfo->IRQn_Channel, &ISR_Prio);
        this->ClearAutomaticFlag();

      #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
        m_DMA_IsItBusyTX = false;
      #endif

    }
  #if (SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        m_VirtualUartBusyRX = false;
        m_VirtualUartBusyTX = false;
        ISR_Init(VirtualUartRX_IRQn, &ISR_Prio);
        ISR_Init(VirtualUartTX_IRQn, &ISR_Prio);
    }
  #endif
    else
    {
        m_pInfo = nullptr;
        m_pUart = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Enable
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Enable uart
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::Enable(void)
{
    SET_BIT(m_pUart->CR1, USART_CR1_UE);              // Enable the USART
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           Disable
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Disable uart RX/TX, isr and uart
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::Disable(void)
{
    if(m_pUart != nullptr)
    {
      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        CLEAR_BIT(m_pUart->CR3, USART_CR3_EIE);
      #endif

      #if (UART_ISR_RX_BYTE_CFG == DEF_ENABLED)
        CLEAR_BIT(m_pUart->CR1, USART_CR1_RXNEIE);
      #endif

      #if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED)
        CLEAR_BIT(m_pUart->CR1, USART_CR1_IDLEIE);
      #endif

      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        CLEAR_BIT(m_pUart->CR1, USART_CR1_TXEIE);
      #endif

      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        CLEAR_BIT(m_pUart->CR1, USART_CR1_TCIE);
      #endif

      #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
        this->DMA_DisableRX();
        this->DMA_DisableTX();
      #endif

        CLEAR_BIT(m_pUart->CR1, USART_CR1_UE);    // Disable the UART
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetBaudRate
//
//  Parameter(s):   BaudRate             BaudRate ID
//                  ReEnable             if true the UART will be re enable
//  Return:         None
//
//  Description:    Set UART Baudrate
//
//  Note(s):        - Disable uart
//                  - Set baud rate and uart properties
//                  - Enable uart if ReEnable is true
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::SetBaudRate(UART_Baud_e BaudRate)
{
    uint32_t PeriphClk;

    if(m_pUart != nullptr)
    {
        this->Disable();

        // Retrieve Clock frequency used for USART Peripheral
      #if (UART_DRIVER_SUPPORT_UART1_CFG == DEF_ENABLED)
        if(m_pUart == USART1)
        {
          PeriphClk = 108000000;//SYS_APB2_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART2_CFG == DEF_ENABLED)
        if(m_pUart == USART2)
        {
          PeriphClk = 54000000;//SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART3_CFG == DEF_ENABLED)
        if(m_pUart == USART3)
        {
          PeriphClk = 54000000;//SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART4_CFG == DEF_ENABLED)
        if(m_pUart == UART4)
        {
          PeriphClk = 54000000;//SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART5_CFG == DEF_ENABLED)
        if(m_pUart == UART5)
        {
          PeriphClk = SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART6_CFG == DEF_ENABLED)
        if(m_pUART == USART6)
        {
          PeriphClk = SYS_APB2_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART7_CFG == DEF_ENABLED)
        if(m_pUart == UART7)
        {
          PeriphClk = SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

      #if (UART_DRIVER_SUPPORT_UART8_CFG == DEF_ENABLED)
        if(m_pUart == UART8)
        {
          PeriphClk = SYS_APB1_CLOCK_FREQUENCY;
        }
      #endif

        if((m_pUart->CR1 & UART_CR1_OVERSAMPLING_8) == UART_CR1_OVERSAMPLING_8)
        {
            PeriphClk <<= 1;
        }

        m_pUart->BRR = uint16_t((PeriphClk + (m_BaudRate[BaudRate] >> 1)) / m_BaudRate[BaudRate]);

        this->Enable();
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           SetConfig
//
//  Parameter(s):   Config               Config for UART
//                  BaudID               Baud ID
//  Return:         None
//
//  Description:    Set UART Config
//
//  Note(s):        - Disable uart
//                  - Set baud rate and uart properties
//                  - Enable uart
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::SetConfig(UART_Config_e Config, UART_Baud_e BaudID)
{
    uint32_t       CR1_Register = 0;
    uint32_t       CR2_Register = 0;
    uint32_t       CR3_Register = 0;
    UART_Config_e  MaskedConfig;

    if(m_pUart != nullptr)
    {
        this->Disable();

        // Parity
        MaskedConfig = UART_Config_e(uint32_t(Config) & UART_PARITY_MASK);

        if(MaskedConfig == UART_EVEN_PARITY)
        {
            CR1_Register |= UART_CR1_PARITY_EVEN;
        }
        else if(MaskedConfig == UART_ODD_PARITY)
        {
            CR1_Register |= UART_CR1_PARITY_ODD;
        }

        // Length
        MaskedConfig = UART_Config_e(uint32_t(Config) & UART_LENGTH_MASK);

        if(MaskedConfig == UART_8_LEN_BITS)
        {
            CR1_Register |= UART_CR1_DATA_WIDTH_8B;
        }
        else if(MaskedConfig == UART_9_LEN_BITS)
        {
            CR1_Register |= UART_CR1_DATA_WIDTH_9B;
        }

        // Stop Bits
        MaskedConfig = UART_Config_e(uint32_t(Config) & UART_STOP_MASK);

        switch(MaskedConfig)
        {
            case UART_0_5_STOP_BIT: CR2_Register |= UART_CR2_STOP_0_5B; break;
            case UART_1_5_STOP_BIT: CR2_Register |= UART_CR2_STOP_1_5B; break;
            case UART_2_STOP_BITS:  CR2_Register |= UART_CR2_STOP_2_B;  break;
            default: break;
        }

        // OverSampling 8 or 16
        if((Config & UART_OVER_MASK) == UART_OVER_8)
        {
            CR1_Register |= UART_CR1_OVER_8;
        }

        // RX and TX enable
        MaskedConfig = UART_Config_e(uint32_t(Config) & UART_ENABLE_MASK);

        switch(MaskedConfig)
        {
            case UART_ENABLE_RX:    CR1_Register |= UART_CR1_RX;    break;
            case UART_ENABLE_TX:    CR1_Register |= UART_CR1_TX;    break;
            case UART_ENABLE_RX_TX: CR1_Register |= UART_CR1_RX_TX; break;
            default: break;
        }

        // Flow control
        MaskedConfig = UART_Config_e(uint32_t(Config) & UART_FLOW_MASK);

        switch(MaskedConfig)
        {
            case UART_FLOW_CTS_AND_ISR:  CR3_Register |= UART_CR3_FLOW_CTS_ISR;   // No break here, we enable the CTS also
            case UART_FLOW_CTS:          CR3_Register |= UART_CR3_FLOW_CTS;     break;
            case UART_FLOW_RTS:          CR3_Register |= UART_CR3_FLOW_RTS;     break;
            default: break;
        }

        // Register modification
        MODIFY_REG(m_pUart->CR1, (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_OVER8), CR1_Register);
        MODIFY_REG(m_pUart->CR2, (USART_CR2_STOP),                                               CR2_Register);
        MODIFY_REG(m_pUart->CR3, (USART_CR3_CTSE | USART_CR3_CTSIE | USART_CR3_RTSE),            CR3_Register);

        this->SetBaudRate(BaudID);
    }
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           IsItBusy
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Check if UART is busy
//
//-------------------------------------------------------------------------------------------------
bool UART_Driver::IsItBusy(void)
{
    if(m_pUart != nullptr)
    {
        return m_DMA_IsItBusyTX;
    }
  #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        if((m_VirtualUartBusyRX == true) || (m_VirtualUartBusyTX == true))
        {
            return true;
        }
    }
  #endif

    return false;
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           GetBaudRate
//
//  Parameter(s):   None
//  Return:         uint32_t    Baud rate
//
//  Description:    Return the real value of the baud rate
//
//-------------------------------------------------------------------------------------------------
uint32_t UART_Driver::GetBaudRate(void)
{
    return m_BaudRate[m_pInfo->BaudID];
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           ClearAutomaticFlag
//
//  Parameter(s):   None
//  Return:         None
//
//  Description:    Clear flag PE, FE, NE, ORE, IDLE
//
//  Note(s):        This method is use to clear any of those flag. It also do a read on the data
//                  register
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::ClearAutomaticFlag(void)
{
    volatile uint32_t tmpreg;

    tmpreg = m_pUart->SR;
    VAR_UNUSED(tmpreg);
    tmpreg = m_pUart->DR;
    VAR_UNUSED(tmpreg);
}

//-------------------------------------------------------------------------------------------------
//
//  Name:           VirtualSendData
//
//  Parameter(s):   pBuffer         Pointer on the buffer containing the data to send
//                  Size            Status
//  Return:         None
//
//  Description:    Send data using virtual comm driver with software ISR
//
//-------------------------------------------------------------------------------------------------
#if (UART_SUPPORT_VIRTUAL_CFG == DEF_ENABLED)
void UART_Driver::VirtualSendData(const uint8_t* pBuffer, uint16_t Size)
{
    this->ReceivedFromVirtualUart(pBuffer, Size);
    NVIC_SetPendingIRQ(VirtualUartTX_IRQn);
    m_VirtualUartBusyTX = true;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//   Function:      SendData
//
//   Parameter(s):  p_BufferTX  Ptr on buffer with data to send.
//                              if = NULL, internal TX Buffer remains the one set previously
//                  pSizeTX     Number of bytes to send, and on return, number of bytes sent
//   Return Value:  SystemState_e
//
//   Description:   Send a data packet to the UART in DMA
//
//-------------------------------------------------------------------------------------------------
SystemState_e UART_Driver::SendData(const uint8_t* pBufferTX, size_t* pSizeTX, void* pContext)
{
    SystemState_e       State = SYS_READY;
    DMA_Stream_TypeDef* pDMA;
    uint32_t            Flag;

    if(m_pUart != nullptr)
    {
        if(*pSizeTX != 0)
        {
            if(m_DMA_IsItBusyTX == false)
            {
                m_DMA_IsItBusyTX = true;
                pDMA             = m_pDMA_Info->DMA_StreamTX;
                Flag             = m_pDMA_Info->FlagTX;

                DMA_Disable(pDMA);
                DMA_ClearFlag(pDMA, Flag);

                if(pBufferTX != nullptr)
                {
                    pDMA->M0AR = (uint32_t)pBufferTX;
                    pDMA->NDTR = *pSizeTX;
                    m_Variables.SizeTX    = *pSizeTX;
                    m_Variables.pBufferTX = (uint8_t*)pBufferTX;
                }
                else
                {
                    pDMA->M0AR = (uint32_t)m_Variables.pBufferTX;
                    pDMA->NDTR = m_Variables.SizeTX;
                }

                m_pContextTX = pContext;

                this->DMA_EnableTX();
                DMA_Enable(pDMA);                    // Transmission starts as soon as TXE is detected
            }
            else
            {
                State = SYS_BUSY;
            }
        }
        else
        {
            State = SYS_READY;
        }
    }
  #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        nOS_EnterCritical();

        if(m_VirtualUartBusyRX == false)
        {
            m_VirtualUartBusyRX = true;
            nOS_LeaveCritical();

            memcpy(m_pDMA_BufferRX, pBuffer, Size);
            m_Variables.SizeRX = Size;
            NVIC_SetPendingIRQ(VirtualUartRX_IRQn);
            state = SYS_READY;
        }
        else
        {
            state =  SYS_BUSY;
        }

        nOS_LeaveCritical();

  #endif
    else
    {
        State = SYS_WRONG_VALUE;
    }

    return State;
}

//-------------------------------------------------------------------------------------------------
//  DDDDDD  MM     MM  AAAAA
//  DD   DD MMMM MMMM AA   AA
//  DD   DD MM MMM MM AAAAAAA
//  DD   DD MM  M  MM AA   AA
//  DDDDDD  MM     MM AA   AA
//-------------------------------------------------------------------------------------------------

#if (UART_DRIVER_ANY_DMA_OR_VIRTUAL_CFG == DEF_ENABLED)
//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_ConfigRX
//
//   Parameter(s):  pBufferRX   Ptr on buffer to store data received.
//                              if pBufferRX = nullptr, pBufferRX remains the one set previously
//                  SizeRX      Number of data max to received
//   Return Value:  None
//
//   Description:   Config the RX DMA to receive data
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_ConfigRX(uint8_t* pBufferRX, size_t SizeRX)
{
    DMA_Stream_TypeDef* pDMA;

    if(m_pUart != nullptr)
    {
        DMA_DisableRX();
        pDMA = m_pDMA_Info->DMA_StreamRX;

        if(pBufferRX != nullptr)
        {
            pDMA->M0AR = (uint32_t)pBufferRX;
            pDMA->NDTR = SizeRX;
            m_Variables.pBufferRX    = pBufferRX;
            m_Variables.SizeRX       = SizeRX;
            m_Variables.StaticSizeRX = SizeRX;
        }
        else
        {
            pDMA->M0AR = (uint32_t)m_Variables.pBufferRX;
            pDMA->NDTR = m_Variables.StaticSizeRX;
            m_Variables.SizeRX = m_Variables.StaticSizeRX;
        }

        this->DMA_EnableRX();
    }
  #if (SUPPORT_VIRTUAL_UART == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        if(pBufferRX != nullptr)
        {
            m_Variables.SizeRX       = SizeRX;
            m_Variables.StaticSizeRX = SizeRX;
            m_Variables.pBufferRX    = pBufferRX;
        }
        else
        {
            m_Variables.SizeRX = m_Variables.StaticSizeRX;
        }
    }
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_ConfigTX
//
//   Parameter(s):  pBufferRX       Pointer on the RX buffer
//                  SizeRX          Size of RX buffer
//                  pBufferTX       Pointer on the TX buffer
//                  SizeTX          Size of TX buffer
//
//   Return Value:  None
//
//   Description:   Initialization of UART DMA
//
//   Note(s):
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_ConfigTX(uint8_t* pBufferTX, size_t SizeTX)
{
    DMA_Stream_TypeDef* pDMA;

    if(m_pUart != nullptr)
    {
        DMA_DisableTX();
        pDMA = m_pDMA_Info->DMA_StreamTX;

        if(pBufferTX != nullptr)
        {
            pDMA->M0AR = (uint32_t)pBufferTX;
            pDMA->NDTR = SizeTX;
            m_Variables.pBufferTX    = pBufferTX;
            m_Variables.SizeTX       = SizeTX;
            m_Variables.StaticSizeTX = SizeTX;
        }
        else
        {
            pDMA->M0AR = (uint32_t)m_Variables.pBufferTX;
            pDMA->NDTR = m_Variables.StaticSizeTX;
            m_Variables.SizeTX = m_Variables.StaticSizeTX;
        }

        this->DMA_EnableTX();
    }

  #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        if(pBufferTX != nullptr)
        {
            m_Variables.pBufferTX    = pBufferTX;
            m_Variables.SizeTX       = SizeTX;
            m_Variables.StaticSizeTX = SizeTX;
        }
        else
        {
            m_Variables.SizeTX = m_Variables.StaticSizeTX;
        }
    }
  #endif
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_EnableRX
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Enable DMA receive
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_EnableRX(void)
{
    DMA_Stream_TypeDef* pDMA;

    if(m_pUart != nullptr)
    {
        if(m_pDMA_Info != nullptr)
        {
            pDMA = m_pDMA_Info->DMA_StreamRX;
            SET_BIT(m_pUart->CR3, USART_CR3_DMAR);      // Enable the DMA transfer
            (void)m_pUart->DR;
            DMA_ClearFlag(pDMA, m_pDMA_Info->FlagRX);
            DMA_Enable(pDMA);
            this->EnableRX_ISR(UART_ISR_RX_ERROR_MASK | UART_ISR_RX_IDLE_MASK);
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_DisableRX
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Disable DMA receive
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_DisableRX(void)
{
    DMA_Stream_TypeDef* pDMA;

    if(m_pUart != nullptr)
    {
        if(m_pDMA_Info != nullptr)
        {
            pDMA = m_pDMA_Info->DMA_StreamRX;
            DMA_Disable(pDMA);
            CLEAR_BIT(m_pUart->CR3, USART_CR3_DMAR);
            this->DisableRX_ISR(UART_ISR_RX_ERROR_MASK | UART_ISR_RX_IDLE_MASK);
            DMA_ClearFlag(pDMA, m_pDMA_Info->FlagRX);
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_EnableTX
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Enable DMA transmit
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_EnableTX(void)
{
    if(m_pUart != nullptr)
    {
        if(m_pDMA_Info != nullptr)
        {
            SET_BIT(m_pUart->CR3, USART_CR3_DMAT);
            this->EnableTX_ISR(UART_ISR_TX_COMPLETED_MASK);
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_DisableTX
//
//   Parameter(s):  None
//   Return Value:  None
//
//   Description:   Disable DMA transmit
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::DMA_DisableTX(void)
{
    DMA_Stream_TypeDef* pDMA;

    if(m_pUart != nullptr)
    {
        if(m_pDMA_Info != nullptr)
        {
            pDMA = m_pDMA_Info->DMA_StreamTX;
            DMA_Disable(pDMA);
            DMA_ClearFlag(pDMA, m_pDMA_Info->FlagTX);
            CLEAR_BIT(m_pUart->CR3, USART_CR3_DMAT);
        }
    }
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_GetInfoRX
//
//   Parameter(s):  None
//   Return Value:  UART_Variables_t*      Pointer on the DMA info
//
//   Description:   return the actual data struct for DMA transfer
//
//-------------------------------------------------------------------------------------------------
UART_Variables_t* UART_Driver::GetInfoRX(void)
{
    if(m_UartID <  NB_OF_UART_DRIVER)
    {
        return &m_Variables;
    }
    return nullptr;
}

//-------------------------------------------------------------------------------------------------
//
//   Function:      DMA_GetSizeRX
//
//   Parameter(s):  None
//   Return Value: size_t       Number of bytes
//
//   Description:   Get number of bytes received
//
//-------------------------------------------------------------------------------------------------
size_t UART_Driver::DMA_GetSizeRX(uint16_t SizeRX)
{
    DMA_Stream_TypeDef* pDMA;
    size_t              SizeDataRX = 0;

    if(m_pUart != nullptr)
    {
        pDMA           = m_pDMA_Info->DMA_StreamRX;

        DMA_Disable(pDMA);
        DMA_ClearFlag(pDMA, m_pDMA_Info->FlagRX);
        SizeDataRX = pDMA->NDTR;                     // Number of byte available in p_RxBuf

        if(SizeDataRX <= SizeRX)
        {
            SizeDataRX = (uint16_t)(SizeRX - SizeDataRX);
        }
        else
        {
            SizeDataRX = SizeRX;
        }

        DMA_Enable(pDMA);
    }
  #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
    else if(m_UartID == UART_DRIVER_VIRTUAL)
    {
        SizeDataRX = m_VirtualVar.SizeRX;
    }
  #endif

    return SizeDataRX;
}

//-------------------------------------------------------------------------------------------------

#endif // UART_DRIVER_ANY_DMA_OR_VIRTUAL_CFG == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
//  IIII RRRRRR    QQQQQ
//   II  RR   RR  QQ   QQ
//   II  RRRRRR   QQ   QQ
//   II  RR  RR   QQ  QQQ
//  IIII RR   RR   QQQQ QQ
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//
//   Function:      EnableRX_ISR
//
//   Parameter(s):  Mask        RX ISR request
//                                  UART_ISR_RX_ERROR_MASK
//                                  UART_ISR_RX_BYTE_MASK
//                                  UART_ISR_RX_TIMEOUT_MASK
//   Return Value:  None
//
//   Description:   Enable specific receive interrupt
//
//-------------------------------------------------------------------------------------------------
#if (UART_ISR_RX_IDLE_CFG  == DEF_ENABLED) || (UART_ISR_RX_ERROR_CFG == DEF_ENABLED) || (UART_ISR_RX_CFG == DEF_ENABLED)
void UART_Driver::EnableRX_ISR(uint8_t Mask)
{
    volatile uint32_t Register;

    if(m_pUart != nullptr)
    {
        // Not empty, Idle, Error flag (Overrun, Framing error, Parity eroor)
        CLEAR_BIT(m_CopySR, (USART_SR_RXNE | USART_SR_IDLE | USART_SR_ORE | USART_SR_FE | USART_SR_PE));

      #if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_IDLE_MASK) != 0)
        {
            SET_BIT(m_CopySR, USART_SR_IDLE);
            Register = m_pUart->SR;
            (void)Register;
            Register = m_pUart->DR;
            (void)Register;
            SET_BIT(m_pUart->CR1, USART_CR1_IDLEIE);
        }
      #endif

      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_ERROR_MASK) != 0)
        {
            SET_BIT(m_CopySR, (USART_SR_ORE | USART_SR_FE | USART_SR_PE));
            this->ClearAutomaticFlag();
            SET_BIT(m_pUart->CR3, USART_CR3_EIE);
        }
      #endif

/*
      #if (UART_ISR_RX_BYTE_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_BYTE_MASK) != 0)
        {
            SET_BIT(m_CopyISR, USART_ISR_RXNE);
            SET_BIT(m_pUart->CR1, USART_CR1_RXNEIE);
        }
      #endif
*/
    }
}
#endif

//-------------------------------------------------------------------------------------------------
//
//   Function:      DisableRX_ISR
//
//   Parameter(s):  Mask        TX ISR request
//                                  UART_ISR_RX_BYTE_MASK
//                                  UART_ISR_RX_TIMEOUT_MASK
//   Return Value:  None
//
//   Description:   Enable specific receive interrupt
//
//-------------------------------------------------------------------------------------------------
#if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED) || (UART_ISR_RX_ERROR_CFG == DEF_ENABLED) || (UART_ISR_RX_CFG == DEF_ENABLED)
void UART_Driver::DisableRX_ISR(uint8_t Mask)
{
    if(m_pUart != nullptr)
    {
      #if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_IDLE_MASK) != 0)
        {
            CLEAR_BIT(m_CopySR, USART_SR_IDLE);
            CLEAR_BIT(m_pUart->CR1, USART_CR1_IDLEIE);
        }
      #endif

      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_ERROR_MASK) != 0)
        {
            CLEAR_BIT(m_CopySR, (USART_SR_ORE | USART_SR_FE | USART_SR_PE));
            CLEAR_BIT(m_pUart->CR3, USART_CR3_EIE);
        }
      #endif

/*
      #if (UART_ISR_RX_BYTE_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_RX_BYTE_MASK) != 0)
        {
            CLEAR_BIT(m_CopyISR, USART_ISR_RXNE);
            CLEAR_BIT(m_pUart->CR1, USART_CR1_RXNEIE);
        }
      #endif
*/

    }
}
#endif

//-------------------------------------------------------------------------------------------------
//
//   Function:      EnableTX_ISR
//
//   Parameter(s):  Mask        TX ISR request
//                                  UART_ISR_TX_EMPTY_MASK
//                                  UART_ISR_TX_COMPLETE_MASK
//   Return Value:  None
//
//   Description:   Enable specific transmit interrupt
//
//-------------------------------------------------------------------------------------------------
#if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED) || (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
void UART_Driver::EnableTX_ISR(uint8_t Mask)
{
    if(m_pUart != nullptr)
    {
        CLEAR_BIT(m_CopySR, (USART_SR_TC | USART_SR_TXE));    // Clear transmit complete and transmit empty

      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_TX_EMPTY_MASK) != 0)
        {
            SET_BIT(m_CopySR, USART_SR_TXE);
            SET_BIT(m_pUart->CR1, USART_CR1_TXEIE);
        }
      #endif

      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_TX_COMPLETED_MASK) != 0)
        {
            SET_BIT(m_CopySR, USART_SR_TC);
            m_pUart->SR= ~USART_SR_TC;
            SET_BIT(m_pUart->CR1, USART_CR1_TCIE);
        }
      #endif
    }
}
#endif

//-------------------------------------------------------------------------------------------------
//
//   Function:      DisableTX_ISR
//
//   Parameter(s):  Mask        TX ISR request
//                                  UART_ISR_TX_EMPTY_MASK
//                                  UART_ISR_TX_COMPLETE_MASK
//   Return Value:  None
//
//   Description:   Disable specific transmit interrupt
//
//-------------------------------------------------------------------------------------------------
#if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED) || (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
void UART_Driver::DisableTX_ISR(uint8_t Mask)
{
    if(m_pUart != nullptr)
    {
      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_TX_EMPTY_MASK) != 0)
        {
            CLEAR_BIT(m_CopyISR, USART_ISR_TXE);
            CLEAR_BIT(m_pUart->CR1, USART_CR1_TXEIE);
        }
      #endif

      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        if((Mask & UART_ISR_TX_COMPLETED_MASK) != 0)
        {
            CLEAR_BIT(m_CopySR, USART_SR_TC);
            CLEAR_BIT(m_pUart->CR1, USART_CR1_TCIE);
        }
      #endif
    }
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  Name:           RegisterCallback...
//
//  Parameter(s):   pCallback       Callback pointer
//                  pContext        Context for ISR
//  Return:         None
//
//  Description:    Register callback for user code in ISR
//
//-------------------------------------------------------------------------------------------------
#if (UART_ISR_RX_CFG == DEF_ENABLED)
void UART_Driver::RegisterCallbackRX(void* pCallback, void* pContext)
{
    m_pCallbackRX = (UART_CallBack_t)pCallback;
    m_pContextRX  = pContext;
}
#endif

#if UART_ISR_RX_IDLE_CFG == DEF_ENABLED
void UART_Driver::RegisterCallbackIdle(void* pCallback, void* pContext)
{
    m_pCallbackIDLE = (UART_CallBack_t)pCallback;
    m_pContextIDLE  = pContext;
}
#endif

#if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
void UART_Driver::RegisterCallbackError(void* pCallback, void* pContext)
{
    m_pCallbackERROR = (UART_CallBack_t)pCallback;
    m_pContextERROR  = pContext;
}
#endif

#if (UART_ISR_CTS_CFG == DEF_ENABLED)
void UART_Driver::RegisterCallbackCTS(void* pCallback, void* pContext)
{
    m_pCallbackCTS = (UART_CallBack_t)pCallback;
    m_pContextCTS  = pContext;
}
#endif

#if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
void UART_Driver::RegisterCallbackEmptyTX(void* pCallback, void* pContext)
{
    m_pCallbackEmptyTX = (UART_CallBack_t)pCallback;
    m_pContextEmptyTX  = pContext;
}
#endif

#if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
void UART_Driver::RegisterCallbackCompletedTX(void* pCallback, void* pContext)
{
    m_pCallbackCompletedTX = (UART_CallBack_t)pCallback;
    m_pContextCompletedTX  = pContext;
}
#endif

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    IRQHandler
//
//  Description:    This function handle UARTx/USARTx interrupt.
//
//-------------------------------------------------------------------------------------------------
void UART_Driver::IRQ_Handler(void)
{
    uint32_t Status;

    if(m_pUart != nullptr)
    {
        Status  = m_pUart->SR;
        Status &= m_CopySR;

       #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        if((Status & (USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE)) != 0)
        {
            this->ClearAutomaticFlag();

            if(m_pCallbackERROR != nullptr)
            {
                if(m_pContextERROR == nullptr) m_pCallbackERROR(m_pContextRX);
                else                           m_pCallbackERROR(m_pContextERROR);
            }

            m_DMA_IsItBusyTX = false;
        }
       #endif

       #if (UART_ISR_RX_BYTE_CFG == DEF_ENABLED)
        if((Status & USART_ISR_RXNE) != 0)
        {
            WRITE_REG(m_pUart->SR, ~(USART_SR_RXNE));

            if(m_pCallbackRX != nullptr)
            {
                uint16_t* Data;

                Data = READ_BIT(m_pUart->DR, USART_RDR_RDR_Msk);
                m_pCallbackRX(READ_BIT(&Data);
            }
        }
       #endif

        #if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED)
         if((Status & USART_SR_IDLE) != 0)
         {
           #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
             m_Variables.SizeRX -= m_pDMA_Info->DMA_StreamRX->NDTR; // Give actual position in the DMA Buffer
           #endif

             this->ClearAutomaticFlag();

             if(m_pCallbackIDLE != nullptr)
             {
                if(m_pContextIDLE == nullptr) m_pCallbackIDLE(m_pContextRX);
                else                          m_pCallbackIDLE(m_pContextIDLE);
             }
             else
             {
               #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
                 this->DMA_ConfigRX(nullptr, 0); // Reset RX packet to avoid override with a new RX packet
               #endif
             }
         }
        #endif

     #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        if((Status & USART_ISR_TXE) != 0)
        {
            if(m_pCallbackEmptyTX != nullptr)
            {
                if(m_pContextEmptyTX == nullptr) m_pCallbackEmptyTX(m_pContextTX);
                else                             m_pCallbackEmptyTX(m_pContextEmptyTX);
            }
        }
       #endif

       #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        if((Status & USART_SR_TC) != 0)
        {
            WRITE_REG(m_pUart->SR, ~(USART_SR_TC));

            if(m_pCallbackCompletedTX != nullptr)
            {
                if(m_pContextCompletedTX == nullptr) m_pCallbackCompletedTX(m_pContextTX);
                else                                 m_pCallbackCompletedTX(m_pContextCompletedTX);
            }

            this->DMA_DisableTX();
            m_DMA_IsItBusyTX = false;
        }
       #endif
    }
}

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    VirtualUartRX_IRQHandler
//
//  Description:    This function handles virtual UART interrupt.
//
//-------------------------------------------------------------------------------------------------
#if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
void UART_Driver::VirtualUartRX_IRQHandler(void)
{
	if(m_pContextCompletedTX == nullptr) m_pCallbackCompletedTX(m_pContextTX);
     else
     {
           m_pCallbackCompletedTX(m_pContextCompletedTX);
//         this->DMA_ConfigRX(nullptr, 0);     // Reset RX packet to avoid override with a new RX packet
     }

     m_VirtualUartBusyRX = false;

#endif

//-------------------------------------------------------------------------------------------------
//
//  IRQ Handler:    VirtualUartTX_IRQHandler
//
//  Description:    This function handles virtual UART interrupt.
//
//-------------------------------------------------------------------------------------------------
#if (SUPPORT_VIRTUAL_UART == DEF_ENABLED)
void UART_Driver::VirtualUartTX_IRQHandler(void)
{
	if(m_pContextCompletedTX == nullptr) m_pCallbackCompletedTX(m_pContextTX);
	else                                 m_pCallbackCompletedTX(m_pContextCompletedTX);

    m_VirtualUartBusyTX = false;
}
#endif

//-------------------------------------------------------------------------------------------------

#endif // USE_UART_DRIVER == DEF_ENABLED
