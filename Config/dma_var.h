//-------------------------------------------------------------------------------------------------
//
//  File :  dma_var.h
//
//-------------------------------------------------------------------------------------------------
//
// Copyright(c) 2024 Alain Royer.
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
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef DMA_DRIVER_GLOBAL

// WS281x led stream will use Timer2 DMA Stream 5 Channel 3
const DMA_Info_t DMA_Info_WS281x =
{
    // Configuration
    DMA_MODE_CIRCULAR                |
    DMA_MEMORY_TO_PERIPHERAL         |
    DMA_PERIPHERAL_NO_INCREMENT      |
    DMA_MEMORY_INCREMENT             |
    DMA_PERIPHERAL_SIZE_16_BITS      |
    DMA_MEMORY_SIZE_8_BITS           |
    DMA_PRIORITY_LEVEL_HIGH          |
    DMA_CHANNEL_3,                          // Connected to channel 3
    DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3,    // Transfer complete and Half transfer Flag
    DMA1_Stream5,                           // DMA_Stream_TypeDef
    DMA1_Stream5_IRQn
    //4,                                      // PreempPrio
};

#endif // DMA_DRIVER_GLOBAL

//-------------------------------------------------------------------------------------------------
