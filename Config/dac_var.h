//-------------------------------------------------------------------------------------------------
//
//  File :  dac_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2023 Alain Royer.
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
//  Notes: DAC variables for STM32F746NG
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

const DAC_Info_t DAC_Driver::m_Info =
{
    DAC_ALIGN_12B_RIGHT,
    DAC_TRIGGER_SOFTWARE,
    DAC_OUTPUT_BUFFER_DISABLE,

  #if (DAC_DRIVER_SUPPORT_DMA_CFG == DEF_ENABLED)
    DMA2,                       // DMA Module
	RCC_APB1ENR_DMA2EN,         // RCC_APBxENR for DMA
    RCC_AHB1ENR_DMA1EN,         // RCC_APBxPeriph

    // DAC Channel 1
    DMA_CHANNEL_3               // DMA_Channel RX
    DMA_LIFCR_CTCIF0,           // RX_IT_Flag
    DMA2_Stream0,               // RX_DMA_TypeDef
    DMA2_Stream0_IRQn,          // RX_IRQn

    // DAC_Channel 2
    DMA_CHANNEL_3,              // DMA_Channel TX
    DMA_LIFCR_CTCIF3,           // TX_IT_Flag
    DMA2_Stream3,               // TX_DMA_TypeDef
    DMA2_Stream3_IRQn,          // TX_IRQn
  #endif
};

//-------------------------------------------------------------------------------------------------
