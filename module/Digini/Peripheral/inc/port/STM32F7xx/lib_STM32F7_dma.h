//-------------------------------------------------------------------------------------------------
//
//  File : lib_STM32F7_dma.h
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

#ifdef STM32F7_DMA_GLOBAL
    #define STM32F7_DMA_EXTERN
#else
    #define STM32F7_DMA_EXTERN extern
#endif

//-------------------------------------------------------------------------------------------------
// Include file(s)
//-------------------------------------------------------------------------------------------------

#include "stm32f7xx.h"

//-------------------------------------------------------------------------------------------------
// define(s) and macro(s)
//-------------------------------------------------------------------------------------------------

#define DMA_MODE_NORMAL                0x00000000        // Normal Mode
#define DMA_MODE_CIRCULAR              DMA_SxCR_CIRC     // Circular Mode
#define DMA_MODE_PFCTRL                DMA_SxCR_PFCTRL   // Peripheral flow control mode

#define DMA_PERIPH_NO_INCREMENT        0x00000000        // Peripheral increment mode Disable
#define DMA_PERIPH_INCREMENT           DMA_SxCR_PINC     // Peripheral increment mode Enable

#define DMA_MEMORY_NO_INCREMENT        0x00000000        // Memory increment mode Disable
#define DMA_MEMORY_INCREMENT           DMA_SxCR_MINC     // Memory increment mode Enable

#define DMA_P_DATA_ALIGN_BYTE          0x00000000        // Peripheral data alignment : Byte
#define DMA_P_DATA_ALIGN_HALF_WORD     DMA_SxCR_PSIZE_0  // Peripheral data alignment : Half Word
#define DMA_P_DATA_ALIGN_WORD          DMA_SxCR_PSIZE_1  // Peripheral data alignment : Word

#define DMA_M_DATA_ALIGN_BYTE          0x00000000        // Memory data alignment : Byte
#define DMA_M_DATA_ALIGN_HALF_WORD     DMA_SxCR_MSIZE_0  // Memory data alignment : Half Word
#define DMA_M_DATA_ALIGN_WORD          DMA_SxCR_MSIZE_1  // Memory data alignment : Word

#define DMA_P_BURST_SINGLE             0x00000000                              // Peripheral burst single transfer configuration
#define DMA_P_BURST_INC4               DMA_SxCR_PBURST_0                       // Peripheral burst of 4 beats transfer configuration
#define DMA_P_BURST_INC8               DMA_SxCR_PBURST_1                       // Peripheral burst of 8 beats transfer configuration
#define DMA_P_BURST_INC16              (DMA_SxCR_PBURST_0 | DMA_SxCR_PBURST_1) // Peripheral burst of 16 beats transfer configuration

#define DMA_M_BURST_SINGLE             0x00000000                              // Memory burst single transfer configuration
#define DMA_M_BURST_INC4               DMA_SxCR_MBURST_0                       // Memory burst of 4 beats transfer configuration
#define DMA_M_BURST_INC8               DMA_SxCR_MBURST_1                       // Memory burst of 8 beats transfer configuration
#define DMA_M_BURST_INC16              (DMA_SxCR_MBURST_0 | DMA_SxCR_MBURST_1) // Memory burst of 16 beats transfer configuration

#define DMA_CHANNEL_0                  0x00000000
#define DMA_CHANNEL_1                  DMA_SxCR_CHSEL_0
#define DMA_CHANNEL_2                  DMA_SxCR_CHSEL_1
#define DMA_CHANNEL_3                  (DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1)
#define DMA_CHANNEL_4                  DMA_SxCR_CHSEL_2
#define DMA_CHANNEL_5                  (DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0)
#define DMA_CHANNEL_6                  (DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_1)
#define DMA_CHANNEL_7                  (DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_0)
#define DMA_CHANNEL_8                  DMA_SxCR_CHSEL_3
#define DMA_CHANNEL_9                  (DMA_SxCR_CHSEL_3 | DMA_SxCR_CHSEL_0)

#define DMA_PRIORITY_LOW               0x00000000           // Priority level : Low
#define DMA_PRIORITY_MEDIUM            DMA_SxCR_PL_0        // Priority level : Medium
#define DMA_PRIORITY_HIGH              DMA_SxCR_PL_1        // Priority level : High
#define DMA_PRIORITY_VERY_HIGH         DMA_SxCR_PL          // Priority level : Very_High

#define DMA_PERIPH_TO_MEMORY           0x00000000           // Peripheral to memory direction
#define DMA_MEMORY_TO_PERIPH           DMA_SxCR_DIR_0       // Memory to peripheral direction
#define DMA_MEMORY_TO_MEMORY           DMA_SxCR_DIR_1       // Memory to memory direction

//-------------------------------------------------------------------------------------------------
// Function prototype(s)
//-------------------------------------------------------------------------------------------------

void        DMA_ClearFlag                           (DMA_Stream_TypeDef* pDMA, uint32_t Flag);
uint32_t    DMA_CheckFlag                           (DMA_Stream_TypeDef* pDMA, uint32_t Flag);
void        DMA_EnableTransmitCompleteInterrupt     (DMA_Stream_TypeDef* pDMA);
void        DMA_DisableTransmitCompleteInterrupt    (DMA_Stream_TypeDef* pDMA);
void        DMA_Enable                              (DMA_Stream_TypeDef* pDMA);
void        DMA_Disable                             (DMA_Stream_TypeDef* pDMA);

//-------------------------------------------------------------------------------------------------

