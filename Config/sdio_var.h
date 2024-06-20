//-------------------------------------------------------------------------------------------------
//
//  File :  sdio_var.h
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
//  Notes : SDIO variables for STM32F7xx
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define(s)
//-------------------------------------------------------------------------------------------------

#define IFCR_CLEAR_MASK_STREAM3 (DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3)
#define IFCR_CLEAR_MASK_STREAM6 (DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6)

//-------------------------------------------------------------------------------------------------
// Constant(s)
//-------------------------------------------------------------------------------------------------

#ifdef SDIO_DRIVER_GLOBAL

#if (USE_SDIO_DRIVER == DEF_ENABLED)
SDIO_Info_t SDIO_Info =
{
    IO_SD_D0,               // SDIO Data 0 to 3
    IO_SD_D1,
    IO_SD_D2,
    IO_SD_D3,
    IO_SD_CLK,              // SDIO CLK
    IO_SD_CMD,				// Command line to SDIO
    IO_DETECT_SD_CARD,      // Card detect line

    // DMA SDIO RX
    {
        // Configuration
        DMA_MODE_PERIPHERAL_FLOW_CTRL    |
        DMA_PERIPHERAL_TO_MEMORY         |
        DMA_PERIPHERAL_NO_INCREMENT      |
        DMA_MEMORY_INCREMENT             |
        DMA_PERIPHERAL_SIZE_32_BITS      |
        DMA_MEMORY_SIZE_32_BITS          |
        DMA_PRIORITY_LEVEL_VERY_HIGH     |
        DMA_PERIPHERAL_BURST_INC4        |
        DMA_MEMORY_BURST_INC4            |
        DMA_CHANNEL_4,                          // Connected to channel 4
        IFCR_CLEAR_MASK_STREAM3,
        DMA2_Stream3,                           // DMA_Stream_TypeDef
        DMA2_Stream3_IRQn
        //4,                                    // PreempPrio
    },

    // DMA SDIO TX
    {
        // Configuration
        DMA_MODE_PERIPHERAL_FLOW_CTRL    |
        DMA_MEMORY_TO_PERIPHERAL         |
        DMA_PERIPHERAL_NO_INCREMENT      |
        DMA_MEMORY_INCREMENT             |
        DMA_PERIPHERAL_SIZE_32_BITS      |
        DMA_MEMORY_SIZE_32_BITS          |
        DMA_PRIORITY_LEVEL_VERY_HIGH     |
        DMA_PERIPHERAL_BURST_INC4        |
        DMA_MEMORY_BURST_INC4            |
        DMA_CHANNEL_4,                          // Connected to channel 4
        IFCR_CLEAR_MASK_STREAM6,
        DMA2_Stream6,                           // DMA_Stream_TypeDef
        DMA2_Stream6_IRQn
        //4,                                    // PreempPrio
    },
};
#endif

class SDIO_Driver mySDIO(&SDIO_Info);

#else // SDIO_DRIVER_GLOBAL

extern       SDIO_Info_t    SDIO_Info;
extern class SDIO_Driver    mySDIO;

#endif // SDIO_DRIVER_GLOBAL
