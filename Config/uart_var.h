//-------------------------------------------------------------------------------------------------
//
//  File :  uart_var.h
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
//
//  Notes : UART variables for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef UART_DRIVER_GLOBAL

const UART_Info_t UART_Info[NB_OF_REAL_UART_DRIVER] =
{
  #if (UART_DRIVER_SUPPORT_UART1_CFG == DEF_ENABLED)
    {
        USART1,                     // USARTx
        IO_UART1_RX,
        IO_UART1_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART1_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART1_RTS,
      #endif
        RCC_APB2ENR_USART1EN,       // RCC_APBxENR
        &RCC->APB2ENR,              // Register
        USART1_IRQn,                // IRQn_Channel
        7,                          // PreempPrio
        UART_Config_e(UART_CONFIG_N_8_1 | UART_OVER_8),
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART2_CFG == DEF_ENABLED)
    {
        USART2,                     // USARTx
        IO_UART2_RX,
        IO_UART2_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART2_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART2_RTS,
      #endif
        RCC_APB1ENR_USART2EN,       // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        USART2_IRQn,                // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART3_CFG == DEF_ENABLED)
    {
        USART3,                     // USARTx
        IO_UART3_RX,
        IO_UART3_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART3_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART3_RTS,
      #endif
        RCC_APB1ENR_USART3EN,       // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        USART3_IRQn,                // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART4_CFG == DEF_ENABLED)
    {
        UART4,                      // USARTx
        IO_UART4_RX,
        IO_UART4_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART4_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART4_RTS,
      #endif
        RCC_APB1ENR_UART4EN,        // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        UART4_IRQn,                 // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART5_CFG == DEF_ENABLED)
    {
        UART5,                      // USARTx
        IO_UART5_RX,
        IO_UART5_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART5_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART5_RTS,
      #endif
        RCC_APB1ENR_UART5EN,        // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        UART5_IRQn,                 // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART6_CFG == DEF_ENABLED)
    {
        USART6,                     // USARTx
        IO_UART6_RX,
        IO_UART6_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART6_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART6_RTS,
      #endif
        RCC_APB2ENR_USART6EN,       // RCC_APBxENR
        &RCC->APB2ENR,              // Register
        USART6_IRQn,                // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART7_CFG == DEF_ENABLED)
    {
        UART7,                      // USARTx
        IO_UART7_RX,
        IO_UART7_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART7_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART7_RTS,
      #endif
        RCC_APB1ENR_UART7EN,        // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        UART7_IRQn,                 // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART8_CFG == DEF_ENABLED)
    {
        UART8,                      // USARTx
        IO_UART8_RX,
        IO_UART8_TX,
      #if (UART_ISR_CTS_CFG == DEF_ENABLED)
        IO_UART8_CTS,
      #endif
      #if (UART_ISR_RTS_CFG == DEF_ENABLED)
        IO_UART8_RTS,
      #endif
        RCC_APB1ENR_UART8EN,        // RCC_APBxENR
        &RCC->APB1ENR,              // Register
        UART8_IRQn,                 // IRQn_Channel
        7,                          // PreempPrio
        UART_CONFIG_N_8_1,
        UART_BAUD_115200,
    },
  #endif
};

#if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
const UART_DMA_Info_t UART_DMA_Info[NB_OF_UART_DMA_DRIVER] =
{
  #if (UART_DRIVER_SUPPORT_UART1_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_1,
        DMA_CHANNEL_4,              // DMA_Channel RX
        DMA_LIFCR_CTCIF2,           // RX_IT_Flag
        DMA2_Stream2,               // RX_DMA_TypeDef
      // Other choice
        // LL_DMA_CHANNEL_4,        // DMA_Channel RX
        // DMA_HIFCR_CTCIF5,        // RX_IT_Flag
        // DMA2_Stream5,            // RX_DMA_TypeDef
        DMA_CHANNEL_4,              // DMA_Channel TX
        DMA_HIFCR_CTCIF7,           // TX_IT_Flag
        DMA2_Stream7,               // TX_DMA_TypeDef
        DMA2_Stream7_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA2EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART2_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_2,
        DMA_CHANNEL_4,              // DMA_Channel RX
        DMA_HIFCR_CTCIF5,           // RX_IT_Flag
        DMA1_Stream5,               // RX_DMA_TypeDef
        DMA_CHANNEL_4,              // DMA_Channel TX
        DMA_HIFCR_CTCIF6,           // TX_IT_Flag
        DMA1_Stream6,               // TX_DMA_TypeDef
        DMA1_Stream6_IRQn,          // TX_IRQn

        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART3_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_3,
        DMA_CHANNEL_4,              // DMA_Channel RX
        DMA_LIFCR_CTCIF1,           // RX_IT_Flag
        DMA1_Stream1,               // RX_DMA_TypeDef
        DMA_CHANNEL_4,              // DMA_Channel TX
        DMA_LIFCR_CTCIF3,           // TX_IT_Flag
        DMA1_Stream3,               // TX_DMA_TypeDef
        DMA1_Stream3_IRQn,          // TX_IRQn
      // Other choice
        //LL_DMA_CHANNEL_7,         // DMA_Channel TX
        //DMA_HIFCR_CTCIF4,         // TX_IT_Flag
        //DMA1_Stream4,             // TX_DMA_TypeDef
        //DMA1_Stream4_IRQn,        // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART4_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_4,
        DMA_CHANNEL_4,              // DMA_Channel RX
        DMA_LIFCR_CTCIF2,           // RX_IT_Flag
        DMA1_Stream2,               // RX_DMA_TypeDef
        DMA_CHANNEL_4,              // DMA_Channel TX
        DMA_HIFCR_CTCIF4,           // TX_IT_Flag
        DMA1_Stream4,               // TX_DMA_TypeDef
        DMA1_Stream4_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART5_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_5,
        DMA_CHANNEL_4,              // DMA_Channel RX
        DMA_LIFCR_CTCIF0,           // RX_IT_Flag
        DMA1_Stream0,               // RX_DMA_TypeDef
        DMA_CHANNEL_4,              // DMA_Channel TX
        DMA_HIFCR_CTCIF7,           // TX_IT_Flag
        DMA1_Stream7,               // TX_DMA_TypeDef
        DMA1_Stream7_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART6_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_6,
        DMA_CHANNEL_5,              // DMA_Channel RX
        DMA_LIFCR_CTCIF1,           // RX_IT_Flag
        DMA2_Stream1,               // RX_DMA_TypeDef
        DMA2_Stream1_IRQn,          // RX_IRQn
      // Other choice
        // DMA_CHANNEL_5,           // DMA_Channel RX
        // DMA_LIFCR_CTCIF2,        // RX_IT_Flag
        // DMA2_Stream2,            // RX_DMA_TypeDef
        DMA_CHANNEL_5,              // DMA_Channel TX
        DMA_HIFCR_CTCIF6,           // TX_IT_Flag
        DMA2_Stream6,               // TX_DMA_TypeDef
        DMA2_Stream6_IRQn,          // TX_IRQn
      // Other choice
        // DMA_CHANNEL_5,           // DMA_Channel TX
        // DMA_HIFCR_CTCIF7,        // TX_IT_Flag
        // DMA2_Stream7,            // TX_DMA_TypeDef
        // DMA2_Stream7_IRQn,       // TX_IRQn
        RCC_AHB1ENR_DMA2EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART7_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_7,
        DMA_CHANNEL_5,              // DMA_Channel RX
        DMA_LIFCR_CTCIF3,           // RX_IT_Flag
        DMA1_Stream3,               // RX_DMA_TypeDef
        DMA_CHANNEL_5,              // DMA_Channel TX
        DMA_LIFCR_CTCIF1,           // TX_IT_Flag
        DMA1_Stream1,               // TX_DMA_TypeDef
        DMA1_Stream1_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif

  #if (UART_DRIVER_SUPPORT_UART8_DMA_CFG == DEF_ENABLED)
    {
        UART_DRIVER_ID_8,
        DMA_CHANNEL_5,              // DMA_Channel RX
        DMA_HIFCR_CTCIF6,           // RX_IT_Flag
        DMA1_Stream6,               // RX_DMA_TypeDef
        DMA_CHANNEL_5,              // DMA_Channel TX
        DMA_LIFCR_CTCIF0,           // TX_IT_Flag
        DMA1_Stream0,               // TX_DMA_TypeDef
        DMA1_Stream0_IRQn,          // TX_IRQn
        RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph
    },
  #endif
};
#endif

class UART_Driver myUART_Terminal(TERMINAL_SERIAL);

#else // UART_DRIVER_GLOBAL

extern const UART_Info_t UART_Info[NB_OF_REAL_UART_DRIVER];

#if (UART_DRIVER_DMA_CFG == DEF_ENABLED)
extern const UART_DMA_Info_t UART_DMA_Info[NB_OF_UART_DMA_DRIVER];
#endif

extern class UART_Driver myUART_Terminal;

#endif // UART_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
