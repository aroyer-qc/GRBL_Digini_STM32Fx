//-------------------------------------------------------------------------------------------------
//
//  File : lib_class_STM32F7_uart.h
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
#include "lib_typedef.h"
#include "lib_io.h"
#include "lib_isr.h"
#include "lib_dma.h"
#include "uart_cfg.h"
#include "driver_cfg.h"

//-------------------------------------------------------------------------------------------------

#if USE_UART_DRIVER == DEF_ENABLED

//-------------------------------------------------------------------------------------------------
// define(s)
//-------------------------------------------------------------------------------------------------

#define UART_ISR_RX_ERROR_MASK              0x01
#define UART_ISR_RX_MASK                    0x02
#define UART_ISR_RX_IDLE_MASK               0x04
#define UART_ISR_TX_EMPTY_MASK              0x01
#define UART_ISR_TX_COMPLETED_MASK          0x02

//-------------------------------------------------------------------------------------------------
// typedef struct(s) and enum(s)
//-------------------------------------------------------------------------------------------------

enum UART_Baud_e
{
    UART_BAUD_9600,
    UART_BAUD_19200,
    UART_BAUD_38400,
    UART_BAUD_57600,
    UART_BAUD_115200,
    UART_BAUD_230400,
    UART_BAUD_460800,
    UART_BAUD_921600,
    UART_BAUD_1843200,
    NB_OF_BAUD,
};

enum UART_Config_e
{
    UART_NO_PARITY          =   0x0000,
    UART_EVEN_PARITY        =   0x0001,
    UART_ODD_PARITY         =   0x0002,
    UART_PARITY_MASK        =   0x0003,

    UART_8_LEN_BITS         =   0x0000,
    UART_7_LEN_BITS         =   0x0004,
    UART_9_LEN_BITS         =   0x0008,
    UART_LENGTH_MASK        =   0x000C,

    UART_1_STOP_BIT         =   0x0000,
    UART_0_5_STOP_BIT       =   0x0010,
    UART_1_5_STOP_BIT       =   0x0020,
    UART_2_STOP_BITS        =   0x0030,
    UART_STOP_MASK          =   0x0030,

    UART_DATA_ORDER_LSB     =   0x0000,
    UART_DATA_ORDER_MSB     =   0x0040,
    UART_DATA_ORDER_MASK    =   0x0040,

    UART_OVER_16            =   0x0000,
    UART_OVER_8             =   0x0080,
    UART_OVER_MASK          =   0x0080,

    UART_ENABLE_RX_TX       =   0x0000,
    UART_ENABLE_RX          =   0x0100,
    UART_ENABLE_TX          =   0x0200,
    UART_ENABLE_MASK        =   0x0300,

    UART_FLOW_CTS           =   0x0400,
    UART_FLOW_CTS_AND_ISR   =   0x0800,
    UART_FLOW_RTS           =   0x1000,
    UART_FLOW_MASK          =   0x1C00,

    // Some more common config (all LSB with oversampling at 16, with RX and TX)
    UART_CONFIG_N_7_1    =   (UART_NO_PARITY   | UART_7_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_N_8_1    =   (UART_NO_PARITY   | UART_8_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_N_9_1    =   (UART_NO_PARITY   | UART_9_LEN_BITS | UART_1_STOP_BIT),

    UART_CONFIG_E_7_1    =   (UART_EVEN_PARITY | UART_7_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_E_8_1    =   (UART_EVEN_PARITY | UART_8_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_E_9_1    =   (UART_EVEN_PARITY | UART_9_LEN_BITS | UART_1_STOP_BIT),

    UART_CONFIG_O_7_1    =   (UART_ODD_PARITY  | UART_7_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_O_8_1    =   (UART_ODD_PARITY  | UART_8_LEN_BITS | UART_1_STOP_BIT),
    UART_CONFIG_O_9_1    =   (UART_ODD_PARITY  | UART_9_LEN_BITS | UART_1_STOP_BIT),

    UART_CONFIG_N_7_2    =   (UART_NO_PARITY   | UART_7_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_N_8_2    =   (UART_NO_PARITY   | UART_8_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_N_9_2    =   (UART_NO_PARITY   | UART_9_LEN_BITS | UART_2_STOP_BITS),

    UART_CONFIG_E_7_2    =   (UART_EVEN_PARITY | UART_7_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_E_8_2    =   (UART_EVEN_PARITY | UART_8_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_E_9_2    =   (UART_EVEN_PARITY | UART_9_LEN_BITS | UART_2_STOP_BITS),

    UART_CONFIG_O_7_2    =   (UART_ODD_PARITY  | UART_7_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_O_8_2    =   (UART_ODD_PARITY  | UART_8_LEN_BITS | UART_2_STOP_BITS),
    UART_CONFIG_O_9_2    =   (UART_ODD_PARITY  | UART_9_LEN_BITS | UART_2_STOP_BITS),
};

struct UART_Info_t
{
    USART_TypeDef*      pUARTx;
    IO_ID_e             PinRX;
    IO_ID_e             PinTX;
  #if (UART_ISR_CTS_CFG == DEF_ENABLED)
    IO_ID_e             PinCTS;
  #endif
  #if (UART_ISR_RTS_CFG == DEF_ENABLED)
    IO_ID_e             PinRTS;
  #endif
    uint32_t            RCC_APBxPeriph;
    volatile uint32_t*  RCC_APBxEN_Register;
    IRQn_Type           IRQn_Channel;
    uint8_t             PreempPrio;
    UART_Config_e       Config;
    UART_Baud_e         BaudID;
};

#if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
struct UART_DMA_Info_t
{
    UART_ID_e           UartID;
    uint32_t            DMA_ChannelRX;
    uint32_t            FlagRX;
    DMA_Stream_TypeDef* DMA_StreamRX;
    uint32_t            DMA_ChannelTX;
    uint32_t            FlagTX;
    DMA_Stream_TypeDef* DMA_StreamTX;
    IRQn_Type           Tx_IRQn;
    uint32_t            RCC_AHBxPeriph;
};

struct UART_Variables_t
{
    uint8_t*            pBufferRX;
    size_t              SizeRX;
    size_t              StaticSizeRX;
    uint8_t*            pBufferTX;
    size_t              SizeTX;
    size_t              StaticSizeTX;
};
#endif

typedef void    (* UART_CallBack_t)           (void* pContext);

//-------------------------------------------------------------------------------------------------
// class definition(s)
//-------------------------------------------------------------------------------------------------

class UART_Driver
{
    public:

                            UART_Driver                     (UART_ID_e UartID);
        void                SetConfig                       (UART_Config_e Config, UART_Baud_e BaudID);
        void                SetBaudRate                     (UART_Baud_e BaudID);
        uint32_t            GetBaudRate                     (void);

        SystemState_e       SendData                        (const uint8_t* p_BufferTX, size_t* pSizeTX, void* pContext = nullptr);

        bool                IsItBusy                        (void);
        UART_Variables_t*   GetInfoRX                       (void);

      #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
        void                DMA_ConfigRX                    (uint8_t* pBufferRX, size_t SizeRX);
        void                DMA_ConfigTX                    (uint8_t* pBufferTX, size_t SizeTX);
        void                DMA_EnableRX                    (void);
        void                DMA_DisableRX                   (void);
        void                DMA_EnableTX                    (void);
        void                DMA_DisableTX                   (void);
        size_t              DMA_GetSizeRX                   (uint16_t SizeRX);
      #endif

      #if (UART_ISR_RX_CFG == DEF_ENABLED)
        void            RegisterCallbackRX                  (void* pCallback, void* pContext = nullptr);
      #endif
      #if UART_ISR_RX_IDLE_CFG == DEF_ENABLED
        void            RegisterCallbackIdle                (void* pCallback, void* pContext = nullptr);
      #endif
      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        void            RegisterCallbackError               (void* pCallback, void* pContext = nullptr);
      #endif
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        void            RegisterCallbackCTS                 (void* pCallback, void* pContext = nullptr);
      #endif
      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        void            RegisterCallbackEmptyTX             (void* pCallback, void* pContext = nullptr);
      #endif
      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        void            RegisterCallbackCompletedTX         (void* pCallback, void* pContext = nullptr);
      #endif

        void            Enable                              (void);
        void            Disable                             (void);

        void            IRQ_Handler                         (void);

       #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
        void            VirtualUartRX_IRQHandler            (void);
        void            VirtualUartTX_IRQHandler            (void);
      #endif

    private:

        void            ClearAutomaticFlag                  (void);

      #if (UART_ISR_RX_IDLE_CFG  == DEF_ENABLED) || (UART_ISR_RX_ERROR_CFG == DEF_ENABLED) || (UART_ISR_RX_CFG == DEF_ENABLED)
        void            EnableRX_ISR                        (uint8_t Mask);
        void            DisableRX_ISR                       (uint8_t Mask);
      #endif

      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED) || (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        void            EnableTX_ISR                        (uint8_t Mask);
        void            DisableTX_ISR                       (uint8_t Mask);
      #endif

        UART_ID_e                   m_UartID;
        static const uint32_t       m_BaudRate[NB_OF_BAUD];
        UART_Info_t*                m_pInfo;
        USART_TypeDef*              m_pUart;
        UART_Variables_t            m_Variables;
        void*                       m_pContextRX;       // This is the global context if there is no individual context set
        void*                       m_pContextTX;       // This is the global context if there is no individual context set

        // DMA Config
      #if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
        UART_DMA_Info_t*            m_pDMA_Info;
        bool                        m_DMA_IsItBusyTX;
      #endif

      #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
        bool                        m_VirtualUartBusyRX;
        bool                        m_VirtualUartBusyTX;
      #endif

      #if (UART_ISR_RX_CFG == DEF_ENABLED)
        UART_CallBack_t             m_pCallbackRX;
        //void*                       m_pContextRX;
      #endif
      #if (UART_ISR_RX_IDLE_CFG == DEF_ENABLED)
        UART_CallBack_t             m_pCallbackIDLE;
        void*                       m_pContextIDLE;
      #endif
      #if (UART_ISR_RX_ERROR_CFG == DEF_ENABLED)
        UART_CallBack_t         m_pCallbackERROR;
        void*                       m_pContextERROR;
      #endif
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        UART_CallBack_t             m_pCallbackCTS;
        void*                       m_pContextCTS;
      #endif
      #if (UART_ISR_TX_EMPTY_CFG == DEF_ENABLED)
        UART_CallBack_t             m_pCallbackEmptyTX;
        void*                       m_pContextEmptyTX;
      #endif
      #if (UART_ISR_TX_COMPLETED_CFG == DEF_ENABLED)
        UART_CallBack_t             m_pCallbackCompletedTX;
        void*                       m_pContextCompletedTX;
      #endif
};

//-------------------------------------------------------------------------------------------------
// constant data
//-------------------------------------------------------------------------------------------------

#include "uart_var.h"         // Project variable

//-------------------------------------------------------------------------------------------------

#endif // USE_UART_DRIVER == DEF_ENABLED
