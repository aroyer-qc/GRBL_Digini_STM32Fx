//-------------------------------------------------------------------------------------------------
//
//  File :  uart_cfg.h
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
// Define(s)
//-------------------------------------------------------------------------------------------------

#define UART_ISR_RX_CFG                         DEF_DISABLED                // rename to RX bytes
#define UART_ISR_RX_ERROR_CFG                   DEF_DISABLED
#define UART_ISR_RX_IDLE_CFG                    DEF_ENABLED
#define UART_ISR_TX_EMPTY_CFG                   DEF_DISABLED
#define UART_ISR_TX_COMPLETED_CFG               DEF_ENABLED
#define UART_ISR_CTS_CFG                        DEF_DISABLED                // Not tested
#define UART_ISR_RTS_CFG                        DEF_DISABLED                // Not tested

#define NB_OF_UART_SUPPORTED_CFG                8
#define NB_OF_UART_DMA_SUPPORTED_CFG            8

#define UART_DRIVER_SUPPORT_UART1_CFG            DEF_ENABLED
#define UART_DRIVER_SUPPORT_UART2_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART3_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART4_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART5_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART6_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART7_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART8_CFG            DEF_DISABLED
#define UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG    DEF_DISABLED

#define UART_DRIVER_SUPPORT_UART1_DMA_CFG        DEF_ENABLED
#define UART_DRIVER_SUPPORT_UART2_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART3_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART4_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART5_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART6_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART7_DMA_CFG        DEF_DISABLED
#define UART_DRIVER_SUPPORT_UART8_DMA_CFG        DEF_DISABLED

#define TERMINAL_SERIAL                 UART_DRIVER_ID_1
//#define DEBUG_SERIAL                  UART_DRIVER_ID_3

#if UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED

//    #define INTERNAL_SERIAL                 DRIVER_UART_UART_VIRTUAL

    //-------------------------------------------------------------------------------------------------
    // Virtual UART

    // Hijack interrupt for module we don't use and transform them into software interrupt
    #define VirtualUartTX_IRQHandler            CAN3_TX_IRQHandler
    #define VirtualUartTX_IRQn                  CAN3_TX_IRQn
    #define VirtualUartRX_IRQHandler            CAN2_TX_IRQHandler
    #define VirtualUartRX_IRQn                  CAN2_TX_IRQn

    // Function callback of the user library or application so it can be copied to local buffer
    //          prototype void USER_DataReceived(const uint8_t* pBuffer, uint16_t Size);
    //
    //   Internal UART function call            Outside callback function name
// extern void app_VirtualDataTransmit          (const uint8_t* pBuffer, uint16_t Size);
// #define ReceivedFromVirtualUart              App_VirtualDataTransmit

#endif

//-------------------------------------------------------------------------------------------------
//  typedef(s)
//-------------------------------------------------------------------------------------------------

enum UART_ID_e
{
    #if (UART_DRIVER_SUPPORT_UART1_CFG == DEF_ENABLED)
        UART_DRIVER_ID_1,
    #endif

    #if (UART_DRIVER_SUPPORT_UART2_CFG == DEF_ENABLED)
        UART_DRIVER_ID_2,
    #endif

    #if (UART_DRIVER_SUPPORT_UART3_CFG == DEF_ENABLED)
        UART_DRIVER_ID_3,
    #endif

    #if (UART_DRIVER_SUPPORT_UART4_CFG == DEF_ENABLED)
        UART_DRIVER_ID_4,
    #endif

    #if (UART_DRIVER_SUPPORT_UART5_CFG == DEF_ENABLED)
        UART_DRIVER_ID_5,
    #endif

    #if (UART_DRIVER_SUPPORT_UART6_CFG == DEF_ENABLED)
        UART_DRIVER_ID_6,
    #endif

    #if (UART_DRIVER_SUPPORT_UART7_CFG == DEF_ENABLED)
        UART_DRIVER_ID_7,
    #endif

    #if (UART_DRIVER_SUPPORT_UART8_CFG == DEF_ENABLED)
        UART_DRIVER_ID_8,
    #endif

    NB_OF_REAL_UART_DRIVER,
    INTERNAL_RESYNC_OFFSET = NB_OF_REAL_UART_DRIVER - 1,

    #if (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
        UART_DRIVER_VIRTUAL,
    #endif

     NB_OF_UART_DRIVER,
};

enum UART_DMA_ID_e
{
    #if (UART_DRIVER_SUPPORT_UART1_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_1,
    #endif

    #if (UART_DRIVER_SUPPORT_UART2_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_2,
    #endif

    #if (UART_DRIVER_SUPPORT_UART3_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_3,
    #endif

    #if (UART_DRIVER_SUPPORT_UART4_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_4,
    #endif

    #if (UART_DRIVER_SUPPORT_UART5_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_5,
    #endif

    #if (UART_DRIVER_SUPPORT_UART6_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_6,
    #endif

    #if (UART_DRIVER_SUPPORT_UART7_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_7,
    #endif

    #if (UART_DRIVER_SUPPORT_UART8_DMA_CFG == DEF_ENABLED)
        UART_DMA_DRIVER_ID_8,
    #endif

     NB_OF_UART_DMA_DRIVER,

};

#if (UART_DRIVER_SUPPORT_UART1_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART2_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART3_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART4_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART5_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART6_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART7_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART8_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART9_DMA_CFG  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_UART10_DMA_CFG == DEF_ENABLED)
#define UART_DRIVER_DMA_CFG                        DEF_ENABLED
#else
#define UART_DRIVER_DMA_CFG                        DEF_DISABLED
#endif

#if (UART_DRIVER_DMA_CFG                  == DEF_ENABLED) || \
    (UART_DRIVER_SUPPORT_VIRTUAL_UART_CFG == DEF_ENABLED)
#define UART_DRIVER_ANY_DMA_OR_VIRTUAL_CFG     DEF_ENABLED
#else
#define UART_DRIVER_ANY_DMA_OR_VIRTUAL_CFG     DEF_DISABLED
#endif

//-------------------------------------------------------------------------------------------------
